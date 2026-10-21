import sys

from scripts.parse_graph import _readGrFile, _formatNodeLines, _createNodesFromFormattedNodeLines
from scripts.plot_graph import plotGraphFromRoot


def main(gr_filename: str = "graph.gr", out_name: str = "graph", fmt: str = "svg") -> None:
    node_lines = _readGrFile(gr_filename)
    formatted_lines = _formatNodeLines(node_lines)
    nodes = _createNodesFromFormattedNodeLines(formatted_lines)

    if not nodes:
        print(f"{gr_filename} is empty, run ./build/mygrad first")
        return

    # C++ writes leaves first, so the root is the last node
    root = nodes[-1]
    out_path = plotGraphFromRoot(root, out_name, fmt)
    print(f"Plotted {len(nodes)} nodes from {gr_filename} -> {out_path}")


if __name__ == "__main__":
    main(*sys.argv[1:])
