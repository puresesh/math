void unindex(
    const std::vector<float>& vertices, 
    const std::vector<unsigned int>& indices, 
    std::vector<float>& outputVertices) 
{

    if (vertices.empty() || indices.empty()) {
        throw std::invalid_argument("Vertices and indices must not be empty.");
    }

    size_t numFaces = indices.size() / 3;
    outputVertices.reserve(numFaces * 3 * 3); 

    for (size_t i = 0; i < numFaces; ++i) {
        unsigned int index0 = indices[i * 3];
        unsigned int index1 = indices[i * 3 + 1];
        unsigned int index2 = indices[i * 3 + 2];

        if (index0 * 3 >= vertices.size() || index1 * 3 >= vertices.size() || index2 * 3 >= vertices.size()) {
            throw std::out_of_range("Index out of range for vertices array.");
        }

        outputVertices.push_back(vertices[index0 * 3]);     
        outputVertices.push_back(vertices[index0 * 3 + 1]); 
        outputVertices.push_back(vertices[index0 * 3 + 2]); 

        outputVertices.push_back(vertices[index1 * 3]);     
        outputVertices.push_back(vertices[index1 * 3 + 1]); 
        outputVertices.push_back(vertices[index1 * 3 + 2]); 

        outputVertices.push_back(vertices[index2 * 3]);     
        outputVertices.push_back(vertices[index2 * 3 + 1]); 
        outputVertices.push_back(vertices[index2 * 3 + 2]); 
    }
}
