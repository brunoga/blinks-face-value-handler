#include <blinklib.h>

#include "face_value_handler.h"

// We have a single field which is the game mode, and it starts at the first bit
// (bit 0) in the face value.
#define OFFSET_GAME_MODE 0

// Our only field is obviously the first field, so its index is 0.
#define INDEX_GAME_MODE 0

void setup() {}

void loop() {
  // Create an instance of our FaceValueHandler. As it has no callback, it will
  // automatically propagate to all faces any changes detected in any faces.
  // Note that if multiple changes arrive for the same field in different faces,
  // the change in the highest numbered face will be the one to be propagated.
  //
  // We have a single piece of data we are tracking, the game mode, so we only
  // add  a single offset here.
  FaceValueHandler face_value_handler(nullptr, OFFSET_GAME_MODE);

  // Instantiating the FaceValueHandler above already propagated any changes to
  // all output faces so to know what is our game state at this point we just
  // need to check the value of the game mode field in any of the output faces.
  // We use face 0 here but it could have been any other face.
  byte game_mode = face_value_handler.GetOutputFieldValue(0, INDEX_GAME_MODE);

  // Check if the buttonn on this Blink was pressed (and if it was not to wak
  // the Blink as we want to ignore it in this case).
  if (buttonSingleClicked() && !hasWoken()) {
    // Button cliked. Advance game mode (wrapping around at 4 so the possible
    // values are 0 to 3 inclusive).
    game_mode = (game_mode + 1) % 4;

    // As we updated the game mode, we also need to update it in all output
    // faces.
    face_value_handler.SetOutputFieldValueOnAllFaces(INDEX_GAME_MODE,
                                                     game_mode);
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

  // As the FaceValueHandler instance goes out of scope here, it will
  // automatically update the output value for all faces and cache the previous
  // input value for all faces so the next loop iteration can detect changes.
}
