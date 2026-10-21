import subprocess

from scripts.parse_graph import Node


def _collectDotLines(node: Node, lines: list[str], visited: set[int]) -> None:
    if id(node) in visited:
        return
    visited.add(id(node))

    node_id = f"n{id(node)}"
    lines.append(f'    {node_id} [shape=record, label="{{ {node.label} | data {node.data:.4f} | grad {node.grad:.4f} }}"];')

    if node.op == "none":
        # We are in the leaf
        return

    op_id = f"{node_id}_op"
    lines.append(f'    {op_id} [shape=circle, label="{node.op}"];')
    lines.append(f"    {op_id} -> {node_id};")

    for child in (node.left, node.right):
        if child is not None:
            _collectDotLines(child, lines, visited)
            lines.append(f"    n{id(child)} -> {op_id};")


def plotGraphFromRoot(root: Node, filename: str = "graph", fmt: str = "svg") -> str:
    """
    Renders graph of operations that produced root
    into filename.fmt using graphviz dot
    """
    lines: list[str] = ["digraph G {", "    rankdir=LR;"]
    _collectDotLines(root, lines, set())
    lines.append("}")

    out_path = f"{filename}.{fmt}"
    subprocess.run(
        ["dot", f"-T{fmt}", "-o", out_path],
        input="\n".join(lines),
        text=True,
        check=True,
    )
    return out_path
