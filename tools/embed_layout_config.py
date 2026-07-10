import json
from pathlib import Path

Import("env")

PROJECT_DIR = Path(env["PROJECT_DIR"])
SOURCE_PATH = PROJECT_DIR / "src" / "layout" / "config" / "board_layouts.json"
OUTPUT_PATH = PROJECT_DIR / "include" / "generated" / "LayoutConfigJson.h"


def embed_layout_config(source, target, env):
    raw_json = SOURCE_PATH.read_text(encoding="utf-8")
    json.loads(raw_json)

    OUTPUT_PATH.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT_PATH.write_text(
        "#pragma once\n\n"
        "namespace layout_config {\n"
        "static const char* const kBoardLayoutsJson = R\"LAYOUT_JSON(\n"
        f"{raw_json.rstrip()}\n"
        ")LAYOUT_JSON\";\n"
        "}\n",
        encoding="utf-8",
    )


embed_layout_config(None, None, env)
