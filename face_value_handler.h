#include <blinklib.h>

class FaceValueHandler;

// Prototype for functions that want to respond to any changes in data coming
// through face values. The face parameter is the face where the change was
// detected. The field_index parameter contains the index of the specific field
// that changed. The face_value_handler is a pointer to the handler that
// detected the change and can be used to retrieve the actual change and/or to
// update the output face value data based on the change that just happened. The
// function should return true if it processed the change of false if it did not
// (in this case, the change will be automatically propagated to all faces).
typedef bool (*ChangeCallback)(byte face, byte field_index,
                               FaceValueHandler* face_value_handler);

// Handles detecting changes on input face value fields and setting data to
// output face value fields. The available fields are defined by bit offsets in
// the face value. Note that this class intentionally does not do any error
// checking (to minimize storage space usage) so be sure to use it as described
// in the comments.
class FaceValueHandler {
 public:
  // Constructs a FaceValueHandler instance based on the given parameters. The
  // change_calback parameter is a pointer to a ChangeCallback function that
  // should be called to handle any changes to defined fields. If it is nullptr
  // then the default behavior of propagating changes to all faces will be used.
  // The offset parameter is a variadic one (you can add multiple values) and
  // define the offsets of each field in the face value. The offsets MUST be
  // provided in increasing order and the highest one should be smaller than 6.
  template <typename... Offsets>
  FaceValueHandler(ChangeCallback change_callback, Offsets... offsets)
      : change_callback_(change_callback), offsets_{(byte)offsets..., 6} {
    FOREACH_FACE(face) {
      byte current_face_value = getLastValueReceivedOnFace(face);

      // How many offsets do we have? Note that this will be one more than what
      // the caller gives us as we have 6 hardcoded as the last one.
      byte num_fields = sizeof...(offsets);

      // Check for field changes.
      for (byte field_index = 0; field_index < num_fields; field_index++) {
        byte previous_field_value = GetFieldFromFaceValue(
            previous_input_face_values_[face], field_index);
        byte current_field_value =
            GetFieldFromFaceValue(current_face_value, field_index);

        if (previous_field_value != current_field_value) {
          // Field changed process it.
          if (change_callback == nullptr ||
              !change_callback_(face, field_index, this)) {
            // Either we do not have a callback or we have it but it returned
            // false. Propagate changes to all faces.
            SetOutputFieldValueOnAllFaces(field_index, current_field_value);
          }
        }
      }
    }
  }

  ~FaceValueHandler();

  // Propagate the input value associated with field_index from face to all
  // faces.
  void PropagateData(byte face, byte field_index);

  // Get the input value associated with field_index in the given face.
  byte GetInputFieldValue(byte face, byte field_index);

  // Set the output value associated with the given field_index in the given
  // face.
  void SetOutputFieldValue(byte face, byte field_index, byte value);

  // Set the output value associated with the given field_index in all faces.
  void SetOutputFieldValueOnAllFaces(byte field_index, byte value);

  // Get the output value associated with the given field_index in the given
  // face.
  byte GetOutputFieldValue(byte face, byte field_index);

 private:
  byte GetFieldFromFaceValue(byte value, byte field_index);

  static byte previous_input_face_values_[FACE_COUNT];
  static byte output_face_values_[FACE_COUNT];

  ChangeCallback change_callback_;

  byte offsets_[];
};