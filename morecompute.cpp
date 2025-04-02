#version 430

layout (local_size_x = 256) in; // Number of work items in a work group

layout (std430, binding = 0) buffer InputBuffer {
    float inputData[];
};

layout (std430, binding = 1) buffer OutputBuffer {
    float outputData[];
};

void main() {
    uint id = gl_GlobalInvocationID.x;
    // Perform some mathematical operation
    outputData[id] = inputData[id] * 2.0; // Example operation
}


---

  GLuint inputBuffer, outputBuffer;
std::vector<float> inputData = { /* your data here */ };
std::vector<float> outputData(inputData.size());

// Create input buffer
glGenBuffers(1, &inputBuffer);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, inputBuffer);
glBufferData(GL_SHADER_STORAGE_BUFFER, inputData.size() * sizeof(float), inputData.data(), GL_STATIC_DRAW);

// Create output buffer
glGenBuffers(1, &outputBuffer);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBuffer);
glBufferData(GL_SHADER_STORAGE_BUFFER, outputData.size() * sizeof(float), nullptr, GL_STATIC_DRAW);


---

  GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
const char* shaderSource = /* load your shader source here */;
glShaderSource(computeShader, 1, &shaderSource, nullptr);
glCompileShader(computeShader);

// Check for compilation errors
// ...

GLuint shaderProgram = glCreateProgram();
glAttachShader(shaderProgram, computeShader);
glLinkProgram(shaderProgram);

---

  glUseProgram(shaderProgram);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, inputBuffer);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, outputBuffer);

// Calculate the number of work groups
GLuint numWorkGroups = (inputData.size() + 255) / 256; // Assuming local_size_x = 256
glDispatchCompute(numWorkGroups, 1, 1);
glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT); // Ensure writes are finished

---

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBuffer);
float* ptr = (float*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
if (ptr) {
    std::copy(ptr, ptr + outputData.size(), outputData.begin());
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}
----

    int work_grp_cnt[3];

glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

printf("max global (total) work group counts x:%i y:%i z:%i\n",
  work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);
