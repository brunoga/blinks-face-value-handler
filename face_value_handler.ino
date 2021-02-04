#include <blinklib.h>

#include "face_value_handler.h"

#define OFFSET_GAME_MODE 0

#define INDEX_GAME_MODE 0

void setup() {}

void loop() {
  FaceValueHandler face_value_handler(nullptr, OFFSET_GAME_MODE);

  byte game_mode = face_value_handler.GetOutputFieldValue(0, INDEX_GAME_MODE);

  if (buttonSingleClicked()) {
    game_mode = (game_mode + 1) % 4;
    face_value_handler.SetOutputFieldValueOnAllFaces(INDEX_GAME_MODE,
                                                     game_mode);
  }

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
}
