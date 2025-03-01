#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "crop_model.h"  // Include the model header file (your converted .h file)
#include "scalar.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace {
  const tflite::Model *model = nullptr;
  tflite::MicroInterpreter *interpreter = nullptr;
  TfLiteTensor *input = nullptr;
  TfLiteTensor *output = nullptr;
  int inference_count = 0;

  constexpr int kTensorArenaSize = 4096;  // Increase the tensor arena size (adjust as needed)
  uint8_t tensor_arena[kTensorArenaSize];  // Memory allocation for the model's tensors
}  // namespace

void setup() {
  Serial.begin(115200);
  Serial.println("Starting TensorFlow Lite inference...");

  // Map the model into a usable data structure.
  model = tflite::GetModel(g_model);  // Make sure g_model is the correct variable name from your header file
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    MicroPrintf(
      "Model provided is schema version %d not equal to supported "
      "version %d.",
      model->version(), TFLITE_SCHEMA_VERSION
    );
    return;
  }

  // Pull in only the operation implementations we need.
  static tflite::MicroMutableOpResolver<5> resolver;  // Adjust the number of operations as needed
  if (resolver.AddFullyConnected() != kTfLiteOk) {
    Serial.println("Failed to add FullyConnected operation");
    return;
  }
  if (resolver.AddSoftmax() != kTfLiteOk) {  // Add Softmax if needed
    Serial.println("Failed to add Softmax operation");
    return;
  }
  // Add any other operations required by your model here, like AddConv2D, etc.

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    Serial.println("AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);
  inference_count = 0;
}

////////////////////////////scalar//////////////////////////////
void scaleInputs(float *inputs) {
  // Standard scaling: (x - mean) / std
  for (int i = 0; i < 7; i++) {
    inputs[i] = (inputs[i] - means[i]) / std_devs[i];
  }
}
//////////////////////////////////////////////////////

// Plant names corresponding to the output indices
const char* plantNames[] = {
  "rice", "maize", "chickpea", "kidneybeans", "pigeonpeas", "mothbeans", "mungbean", 
  "blackgram", "lentil", "pomegranate", "banana", "mango", "grapes", "watermelon", 
  "muskmelon", "apple", "orange", "papaya", "coconut", "cotton", "jute", "coffee"
};

void loop() {
  // Wait for serial input (you can send values in the format "x1,x2,x3,x4,x5,x6,x7")
  Serial.println("Enter 7 float values separated by commas:");

  // Wait until input is available
  while (Serial.available() == 0) {
    delay(100);  // Wait until input is available
  }

  // Read the input from the serial
  String inputString = Serial.readStringUntil('\n');  // Read the entire line until newline
  inputString.trim();  // Remove any leading/trailing whitespace

  // Split the string into values based on commas
  float inputs[7];
  int index = 0;
  int lastIndex = 0;

  for (int i = 0; i < inputString.length(); i++) {
    if (inputString.charAt(i) == ',' || i == inputString.length() - 1) {
      String value = inputString.substring(lastIndex, i);
      inputs[index] = value.toFloat();
      index++;
      lastIndex = i + 1;
    }
  }

  // Check if we got the correct number of inputs
  if (index != 7) {
    Serial.println("Incorrect number of inputs. Please enter exactly 7 values.");
    return;  // Stop processing if we didn't get the correct number of inputs
  }

  // Scale the inputs
  scaleInputs(inputs);

  // Feed the scaled input values into the model's input tensor
  for (int i = 0; i < 7; i++) {
    input->data.f[i] = inputs[i];  // Assuming model expects float values
  }

  // Run inference
  if (interpreter->Invoke() != kTfLiteOk) {
    Serial.println("Inference failed");
    return;
  }

  // Find the index of the highest output value
  float maxVal = output->data.f[0];
  int maxIndex = 0;
  for (int i = 1; i < 22; i++) {
    if (output->data.f[i] > maxVal) {
      maxVal = output->data.f[i];
      maxIndex = i;
    }
  }

  // Print the corresponding plant name
  Serial.print("Predicted Plant: ");
  Serial.println(plantNames[maxIndex]);

  delay(1000);  // Optional delay between inferences
}
