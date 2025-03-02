#include <Wire.h>
#include <U8g2lib.h>
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"


#include "crop_model.h"  // Include your TensorFlow Lite model converted to .h
#include "scalar.h"      // Include file for input scaling


// Globals for TensorFlow Lite
namespace {
  const tflite::Model *model = nullptr;
  tflite::MicroInterpreter *interpreter = nullptr;
  TfLiteTensor *input = nullptr;
  TfLiteTensor *output = nullptr;

  constexpr int kTensorArenaSize = 4096;  // Memory allocation for model
  uint8_t tensor_arena[kTensorArenaSize];
}  // namespace


//////////////////////////////////////////////////////////////////////////
int button=0;  //reset button pin connect other lead of button to ground
// Initialize U8g2 for I2C OLED (change constructor if needed)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock */ 23, /* data */ 22, /* reset=*/ U8X8_PIN_NONE);
//////////////////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(115200);
  Serial.println("Starting TensorFlow Lite inference...");
  u8g2.begin();
  u8g2.enableUTF8Print();

  // Map the model
  model = tflite::GetModel(g_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema version mismatch!");
    return;
  }

  // Set up TensorFlow Lite operations
  static tflite::MicroMutableOpResolver<5> resolver;
  if (resolver.AddFullyConnected() != kTfLiteOk) {
    Serial.println("Failed to add FullyConnected operation");
    return;
  }
  if (resolver.AddSoftmax() != kTfLiteOk) { 
    Serial.println("Failed to add Softmax operation");
    return;
  }

  // Set up the interpreter
  static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("Tensor allocation failed!");
    return;
  }

  input = interpreter->input(0);
  output = interpreter->output(0);

  // Display loading animation
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  for (int i = 0; i <= 100; i += 10) {
    u8g2.clearBuffer();
    u8g2.drawStr(30, 20, "Loading ANN...");
    u8g2.drawFrame(10, 30, 108, 10);
    u8g2.drawBox(12, 32, (i * 104) / 100, 6);
    u8g2.sendBuffer();
    delay(10);
  }

  pinMode(button, INPUT_PULLUP);  // Button setup (internal pull-up)
}

// Scale input values
void scaleInputs(float *inputs) {
  for (int i = 0; i < 7; i++) {
    inputs[i] = (inputs[i] - means[i]) / std_devs[i];
  }
}

// List of plant names (22 classes)
const char* plantNames[] = {
  "Rice", "Maize", "Chickpea", "Kidneybeans", "Pigeonpeas", "Mothbeans", "Mungbean", 
  "Blackgram", "Lentil", "Pomegranate", "Banana", "Mango", "Grapes", "Watermelon", 
  "Muskmelon", "Apple", "Orange", "Papaya", "Coconut", "Cotton", "Jute", "Coffee"
};

void loop() {
  u8g2.clearBuffer();
  u8g2.drawStr(10, 30, "System ready.");
  u8g2.sendBuffer();

  Serial.println("Enter 7 float values separated by commas:");

  // Wait for input
  while (Serial.available() == 0) {
    delay(100);
  }

  // Read serial input
  String inputString = Serial.readStringUntil('\n');
  inputString.trim();

  float inputs[7];
  int index = 0, lastIndex = 0;

  for (int i = 0; i < inputString.length(); i++) {
    if (inputString.charAt(i) == ',' || i == inputString.length() - 1) {
      String value = inputString.substring(lastIndex, i + 1);
      inputs[index++] = value.toFloat();
      lastIndex = i + 1;
    }
  }
  float npk[3];
  npk[0]=inputs[0];
  npk[1]=inputs[1];
  npk[2]=inputs[2];

  if (index != 7) {
    Serial.println("Incorrect number of inputs.");
    return;
  }

  scaleInputs(inputs);

  for (int i = 0; i < 7; i++) {
    input->data.f[i] = inputs[i];
  }

  if (interpreter->Invoke() != kTfLiteOk) {
    Serial.println("Inference failed");
    return;
  }

  // Get prediction
  float maxVal = output->data.f[0];
  int maxIndex = 0;
  for (int i = 1; i < 22; i++) {
    if (output->data.f[i] > maxVal) {
      maxVal = output->data.f[i];
      maxIndex = i;
    }
  }

  Serial.print("Predicted Plant: ");
  Serial.println(plantNames[maxIndex]);

  // Display animation
  u8g2.clearBuffer();
  u8g2.drawStr(10, 30, "Analysing soil!!");
  u8g2.sendBuffer();
  delay(800);

  u8g2.clearBuffer();
  u8g2.drawStr(10, 30, "Making Predictions...");
  u8g2.sendBuffer();
  delay(800);

  // Keep displaying the prediction until button press
  while (digitalRead(0) == HIGH) {
    u8g2.clearBuffer();
    u8g2.drawStr(10, 10, plantNames[maxIndex]);
    u8g2.drawStr(0, 30, "N");
    u8g2.drawStr(50, 30, "P");
    u8g2.drawStr(100, 30, "K");
    u8g2.drawStr(0, 60, "press button to reset");

    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%.1f", npk[0]);
    u8g2.drawStr(0, 40, buffer);
    snprintf(buffer, sizeof(buffer), "%.1f", npk[1]);
    u8g2.drawStr(50, 40, buffer);
    snprintf(buffer, sizeof(buffer), "%.1f", npk[2]);
    u8g2.drawStr(100, 40, buffer);

    u8g2.sendBuffer();
    delay(100);
  }

  Serial.println("Button pressed! Ready for new input.");
  delay(300);  // Debounce
}
