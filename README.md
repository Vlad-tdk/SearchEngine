
# Search Engine Project

This project implements a search engine for a corporate portal, allowing users to search through a set of documents for relevant information. The search engine processes documents, indexes them, and returns the most relevant results based on user queries. C++

## Features

- **Document Indexing**: Efficiently indexes a set of documents to allow fast search queries.
- **Search Queries**: Supports complex queries and ranks results based on relevance.
- **Multi-threading**: Document indexing is performed in separate threads to improve performance.
- **JSON Configuration**: The search engine is configurable via JSON files for easy setup and modification.

## Requirements

- C++ compiler with C++20 support
- CMake 3.29 or higher
- [Google Test](https://github.com/google/googletest) (included via FetchContent)
- [nlohmann/json](https://github.com/nlohmann/json)(included local)

## Installation

1. **Clone the repository**:

   ```bash
   git clone https://github.com/yourusername/SearchEngine.git
   cd SearchEngine
   ```
2. **Dependencies**:
The project requires the nlohmann/json library. It is installed locally. To update it, follow these steps:
Download the nlohmann/json repository as a ZIP file from this link.
Unpack the contents of the archive into the root of the project.
Rename the nlohmann-develop folder to nlohmann_json.

3. **Build the project**:

   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

4. **Copy necessary JSON files**:

   Ensure that `config.json` and `requests.json` are copied to the `build` directory:

   ```bash
   cp ../config.json ../requests.json .
   ```

## Usage

After building the project, you can run the search engine using the following command:

```bash
./SearchEngine
```

The program will output the results in `answers.json` based on the queries provided in `requests.json`. If the version in `config.json` does not match the program's version, the application will terminate with an error message.

### Example JSON Configuration

**config.json**:

```json
{
  "config": {
    "name": "SearchEngine",
    "version": "0.1",
    "max_responses": 5
  },
  "files": [
    "../resources/file001.txt",
    "../resources/file002.txt",
    "../resources/file003.txt",
    "../resources/file004.txt"
  ]
}
```

**requests.json**:

```json
{
  "requests": [
    "example query one",
    "example query two",
    "example query three"
  ]
}
```

## Running Tests

To run the tests:

```bash
./TestRunner
```

This will execute all unit tests using Google Test to ensure the correctness of the search engine.

## License

This project is licensed under the MIT License.

## Authors

- **Vladimir** - Initial work

---

This README provides a comprehensive overview of the project, including installation, usage, and testing instructions.
