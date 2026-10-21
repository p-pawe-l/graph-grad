class Node:
    def __init__(self, data, label, op) -> None:
        self.data = data
        self.label = label
        self.op = op
        
        self.left = None
        self.right = None
        
        
def _readGrFile(filename: str) -> list[str]:
    node_lines: list[str] = []
    with open(filename, mode="r") as file_handle:
        for line in file_handle.readlines():
            node_lines.append(line)
    return node_lines


def _formatNodeLines(node_lines: list[str]):
    formatted_node_lines: list = []
    for line in node_lines:
        line = line.strip()
        formatted_node_lines.append([field.strip() for field in line.split("|")])
    return formatted_node_lines
    

def _createNodesFromFormattedNodeLines(lines: list[list[str]]) -> list[Node]:
    nodes: list[Node] = []
    for nline in lines:
        node: Node = Node(
            data=float(nline[1]),
            label=nline[0],
            op=nline[2]
        )
        for iter_node in nodes:
            if iter_node.label == nline[3]:
                node.left = iter_node
            if iter_node.label == nline[4]:
                node.right = iter_node

        nodes.append(node)
    return nodes 
        