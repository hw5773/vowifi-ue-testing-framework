# VoWiFi Property Extraction with RAG

This repository provides a streamlined framework for extracting VoWiFi (Voice over Wi-Fi) properties from RFC documents using Retrieval-Augmented Generation (RAG).

## File Structure

```
vowifi-property-extraction-with-rag/
├── README.md                    # This file
├── requirements.txt             # Python dependencies
├── vowifi_rag_framework.py     # Main RAG framework
├── eval.py                     # Evaluation framework
├── rfcs/                       # RFC documents
│   ├── 33402-i00.pdf
│   ├── rfc3665.txt.pdf
│   └── rfc5996.pdf
├── extracted_properties/        # Output directory (created automatically)
│   └── vowifi_properties.txt
├── evaluation_results/          # Evaluation results (created automatically)
└── vowifi_vectorstore/         # Vector database (created automatically)
```

### Prerequisites

- Python 3.8 or higher
- At least 8GB RAM (16GB recommended)
- CUDA-compatible GPU (optional, for faster processing)

## Quick Start

### Installation

1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd vowifi-property-extraction-with-rag
   ```

2. **Install dependencies**:
   ```bash
   # Create virtual environment (recommended)
   python -m venv venv
   source venv/bin/activate  # On Windows: venv\Scripts\activate
   
   # Install dependencies
   pip install -r requirements.txt
   ```

## Usage

### 1. Property Extraction from RFCs

The framework automatically extracts VoWiFi properties from RFC documents in the `rfcs/` folder.

#### Basic Usage

```bash
python vowifi_rag_framework.py
```

This will:
- Process all documents in the `rfcs/` folder
- Build a vector knowledge base
- Extract all VoWiFi properties automatically
- Save results to `extracted_properties/vowifi_properties.txt`


### Complete Workflow Example

Here's a complete example of extracting VoWiFi properties:

```python
from vowifi_rag_framework import VoWiFiRAGPipeline, RAGConfig

# Setup and run property extraction
config = RAGConfig(
    documents_dir="rfcs",
    output_dir="extracted_properties"
)

pipeline = VoWiFiRAGPipeline(config)
pipeline.initialize_models()

# Build knowledge base
if not pipeline.load_knowledge_base():
    pipeline.build_knowledge_base()

# Extract properties
properties = pipeline.extract_properties_from_documents()

print(f"Total properties extracted: {len(properties)}")
```
