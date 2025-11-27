import random
import xml.etree.ElementTree as ET
from xml.dom import minidom  # Import for formatting the XML

def generate_tasks(base_id, highest_id, min_difference, num_tasks, output_file):
    """
    Generate random tasks with specified parameters and save them to an XML file.

    :param base_id: The lowest possible ID for start and goal.
    :param highest_id: The highest possible ID for start and goal.
    :param min_difference: The minimum difference between start_id and goal_id.
    :param num_tasks: The number of tasks to generate.
    :param output_file: The file to save the generated tasks.
    """
    root = ET.Element("root")
    used_start_ids = set()  # Track used start IDs
    used_goal_ids = set()   # Track used goal IDs

    for _ in range(num_tasks):
        # Generate a unique start_id
        start_id = random.randint(base_id, highest_id)
        while start_id in used_start_ids:
            start_id = random.randint(base_id, highest_id)
        used_start_ids.add(start_id)

        # Generate a unique goal_id that satisfies the minimum difference
        valid_goal_ids = [i for i in range(base_id, highest_id + 1) 
                          if abs(i - start_id) >= min_difference and i not in used_goal_ids]
        if not valid_goal_ids:
            raise ValueError("Not enough unique goal IDs to satisfy the constraints.")
        goal_id = random.choice(valid_goal_ids)
        used_goal_ids.add(goal_id)

        # Add the agent to the XML
        agent = ET.SubElement(root, "agent")
        agent.set("start_id", str(start_id))
        agent.set("goal_id", str(goal_id))

    # Convert the ElementTree to a string
    rough_string = ET.tostring(root, encoding="utf-8")
    # Format the XML string with minidom
    reparsed = minidom.parseString(rough_string)
    pretty_xml = reparsed.toprettyxml(indent="   ")

    # Write the formatted XML to the output file
    with open(output_file, "w", encoding="utf-8") as f:
        f.write(pretty_xml)

if __name__ == "__main__":
    # Parameters
    base_id = 0          # Lowest possible ID
    highest_id = 87      # Highest possible ID
    min_difference = 20  # Minimum difference between start_id and goal_id
    num_tasks = 25       # Number of tasks to generate
    output_file = "generated_Tasks1.xml"  # Output file name

    # Generate tasks
    generate_tasks(base_id, highest_id, min_difference, num_tasks, output_file)
    print(f"Generated {num_tasks} tasks and saved to {output_file}.")