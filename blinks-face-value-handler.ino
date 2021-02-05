#include <blinklib.h>

#include "face_value_handler.h"

// The first field we are tracking is the game mode. It starts at the first
// bit (bit 0) in the face value.
#define OFFSET_GAME_MODE 0

// The second field we are tracking tells us if Face 0 should be white or not.
// As we need 2 bits for the game mode field above (as we have 4 possible values
// for the game mode), this field must start at offset 2.
#define OFFSET_WHITE_FACE_0 2

// This is our first field, so its index is 0.
#define INDEX_GAME_MODE 0

// This is our second field, so its index is 1.
#define INDEX_WHITE_FACE_0 1

void setup() {}

void loop() {
  // Create an instance of our FaceValueHandler. As it has no callback, it will
  // automatically propagate to all faces any changes detected in any faces.
  // Note that if multiple changes arrive for the same field in different faces,
  // the change in the highest numbered face will be the one to be propagated.
  //
  // We have 2 pieces of data (fields) we are tracking: the game mode and if we
  // want face 0 to be white. So we pass the 2 associated offsets here.
  FaceValueHandler face_value_handler(nullptr, OFFSET_GAME_MODE,
                                      OFFSET_WHITE_FACE_0);

  // Instantiating the FaceValueHandler above already propagated any changes to
  // all output faces so to know what is our game state at this point we just
  // need to check the value of the game mode and white face 0 fields in any of
  // the output faces. We use face 0 here but it could have been any other face.
  byte game_mode = face_value_handler.GetOutputFieldValue(0, INDEX_GAME_MODE);
  bool white_face_0 =
      face_value_handler.GetOutputFieldValue(0, INDEX_WHITE_FACE_0);

  // Make sure we consume all 2 of the flags below in case we have just waken
  // up. Note that consuming double clicks does not work as expected as it needs
  // two loop iterations at least to be detected and hasWoken() will only return
  // true in the first one.
  bool has_woken = hasWoken();
  bool button_single_clicked = buttonSingleClicked();

  // Was this Blink button clicked and it was not to wake it up?
  if (!has_woken && button_single_clicked) {
    // Advance game mode (wrapping around at 4 so the possible values are 0 to 3
    // inclusive).
    game_mode = (game_mode + 1) % 4;

    // As we updated the game mode, we also need to update it in all output
    // faces.
    face_value_handler.SetOutputFieldValueOnAllFaces(INDEX_GAME_MODE,
                                                     game_mode);
  }

  // Let's also check if the button was double clicked.
  if (buttonDoubleClicked()) {
    // It was. Invert the state of white_face_0.
    white_face_0 = !white_face_0;

    // And update it in all faces.
    face_value_handler.SetOutputFieldValueOnAllFaces(INDEX_WHITE_FACE_0,
                                                     white_face_0);
  }

  // Render Blink color based on current game mode.
  switch (game_mode) {
    case 0:
      setColor(RED);
      break;
    case 1:
      setColor(YELLOW);
      break;
    case 2:
      setColor(GREEN);
      break;
    case 3:
      setColor(CYAN);
      break;
  }

  // Should we render face 0 as white?
  if (white_face_0) {
    // Yes. Do it.
    setColorOnFace(WHITE, 0);
  }

  // As the FaceValueHandler instance goes out of scope here, it will
  // automatically update the output value for all faces and cache the previous
  // input value for all faces so the next loop iteration can detect changes.
}
