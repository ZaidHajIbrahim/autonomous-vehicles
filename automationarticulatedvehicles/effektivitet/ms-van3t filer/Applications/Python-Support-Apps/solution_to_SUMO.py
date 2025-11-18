import xml.etree.ElementTree as ET
import math

def load_graphml_nodes_and_edges(graphml_file):
    """
    Load nodes and edges from the GraphML file.
    :param graphml_file: Path to the GraphML file.
    :return: A dictionary of nodes with their coordinates and a dictionary of edges with their sumo_id.
    """
    tree = ET.parse(graphml_file)
    root = tree.getroot()

    nodes = {}
    edges = {}

    # Parse nodes
    for node in root.findall(".//node"):
        node_id = node.get("id")
        sumo_id = node.get("sumo_id")
        coords = node.find(".//data[@key='key0']").text
        x, y = map(float, coords.split(","))
        nodes[node_id] = {"sumo_id": sumo_id, "coords": (x, y)}

    # Parse edges
    for edge in root.findall(".//edge"):
        edge_id = edge.get("id")
        source = edge.get("source")
        target = edge.get("target")
        sumo_id = edge.get("sumo_id")
        edges[edge_id] = {"source": source, "target": target, "sumo_id": sumo_id}

    return nodes, edges

def find_closest_node(x, y, nodes, threshold=0.1):
    """
    Find the closest node to the given coordinates within the threshold.
    :param x: X-coordinate.
    :param y: Y-coordinate.
    :param nodes: Dictionary of nodes with their coordinates.
    :param threshold: Maximum distance to consider a node as a match.
    :return: The ID of the closest node, or None if no match is found.
    """
    closest_node = None
    min_distance = float("inf")
    for node_id, node_data in nodes.items():
        node_x, node_y = node_data["coords"]
        distance = math.sqrt((x - node_x) ** 2 + (y - node_y) ** 2)
        if distance < min_distance and distance <= threshold:
            min_distance = distance
            closest_node = node_id
    return closest_node

def convert_solution_to_sumo_edges(solution_file, graphml_file, threshold=0.1):
    """
    Convert the solver solution to a list of edges by their sumo_id for each agent.
    :param solution_file: Path to the solver solution file.
    :param graphml_file: Path to the GraphML file.
    :param threshold: Threshold for matching coordinates to nodes.
    :return: A dictionary of agents with their list of edges by sumo_id.
    """
    nodes, edges = load_graphml_nodes_and_edges(graphml_file)
    tree = ET.parse(solution_file)
    root = tree.getroot()

    agent_paths = {}

    for agent in root.findall(".//agent"):
        agent_id = agent.get("number")
        agent_edges = []

        # Get the path sections for the agent
        for section in agent.findall(".//section"):
            start_x, start_y = float(section.get("start_i")), float(section.get("start_j"))
            goal_x, goal_y = float(section.get("goal_i")), float(section.get("goal_j"))

            # Find the closest nodes for start and goal
            start_node = find_closest_node(start_x, start_y, nodes, threshold)
            goal_node = find_closest_node(goal_x, goal_y, nodes, threshold)

            if start_node is None or goal_node is None:
                raise ValueError(f"Could not find matching nodes for coordinates: "
                                 f"start=({start_x}, {start_y}), goal=({goal_x}, {goal_y})")

            # Find the edge connecting the start and goal nodes
            for edge_id, edge_data in edges.items():
                if edge_data["source"] == start_node and edge_data["target"] == goal_node:
                    if edge_data["sumo_id"] != "NIL":  # Ignore edges with sumo_id="NIL"
                        agent_edges.append(edge_data["sumo_id"])
                    break

        agent_paths[agent_id] = agent_edges

    return agent_paths

if __name__ == "__main__":
    # File paths
    solution_file = "effektivitet/CCBS Input Output/output1_tasks_log.xml"  # Path to the solver solution file
    graphml_file = "effektivitet/output1.xml"           # Path to the GraphML file

    # Convert solution to SUMO edges
    agent_paths = convert_solution_to_sumo_edges(solution_file, graphml_file, threshold=0.5)

    # Print the results
    for agent_id, edges in agent_paths.items():
        print(f"Agent {agent_id}: {edges}")