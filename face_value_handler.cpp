#include "face_value_handler.h"

FaceValueHandler::~FaceValueHandler() {
  FOREACH_FACE(face) {
    setValueSentOnFace(output_face_values_[face], face);
    previous_input_face_values_[face] = getLastValueReceivedOnFace(face);
  }
}

void FaceValueHandler::PropagateData(byte face, byte field_index) {
  SetOutputFieldValueOnAllFaces(field_index,
                                GetInputFieldValue(face, field_index));
}

byte FaceValueHandler::GetInputFieldValue(byte face, byte field_index) {
  return GetFieldFromFaceValue(getLastValueReceivedOnFace(face), field_index);
}

void FaceValueHandler::SetOutputFieldValue(byte face, byte field_index,
                                           byte value) {
  byte mask = (byte)255 &
              ~((((offsets_[field_index + 1] - offsets_[field_index]) << 1) - 1)
                << offsets_[field_index]);
  output_face_values_[face] &= mask;
  output_face_values_[face] |= (value << offsets_[field_index]);
}

void FaceValueHandler::SetOutputFieldValueOnAllFaces(byte field_index,
                                                     byte value) {
  FOREACH_FACE(face) { SetOutputFieldValue(face, field_index, value); }
}

byte FaceValueHandler::GetOutputFieldValue(byte face, byte field_index) {
  return GetFieldFromFaceValue(output_face_values_[face], field_index);
}

byte FaceValueHandler::GetFieldFromFaceValue(byte value, byte field_index) {
  return (value >> offsets_[field_index]) &
         ((1 << (offsets_[field_index + 1] - offsets_[field_index])) - 1);
}

byte FaceValueHandler::previous_input_face_values_[FACE_COUNT] = {0, 0, 0,
                                                                  0, 0, 0};
byte FaceValueHandler::output_face_values_[FACE_COUNT] = {0, 0, 0, 0, 0, 0};