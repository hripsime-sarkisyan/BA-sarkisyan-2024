import subprocess
import os
from pathlib import Path
import argparse
import time
import json

SERVICE_NAME = "fishnet-sda"  # Adjust to your docker-compose service name
MOUNT_LOCATION = "${HOME}/.fishnet"
SDA_IMAGE = "logru/sda:1.0"
MEMGRAPH_LAB_PORT = 3000

def build_compose_file(mount_location: str = MOUNT_LOCATION) -> str:
    return f"""services:
  memgraph:
    image: memgraph/memgraph
    container_name: memgraphDB
    ports:
      - "7687:7687"
      - "7444:7444"
    command: ["--log-level=TRACE","--query-execution-timeout-sec=0"]
    volumes:
      - memgraph-logs:/var/log/memgraph

  lab:
    image: memgraph/lab:latest
    container_name: memgraph-lab
    ports:
      - "{MEMGRAPH_LAB_PORT}:3000"
    depends_on:
      - memgraph
    environment:
      - QUICK_CONNECT_MG_HOST=memgraph
      - QUICK_CONNECT_MG_PORT=7687   
 
  {SERVICE_NAME}:
    image: {SDA_IMAGE}
    depends_on:
      - memgraph
    container_name: sda-workflow
    volumes:
      - {mount_location}:/data
      - {mount_location}/working-directory:/tmp/fishnet
volumes:
  memgraph-logs:
    driver: local"""

class SDAGraphConstruction:
    def __init__(self, config_file, input_file):
        self.config_file = config_file
        self.input_file = input_file
        self.compose_file = self._write_compose_file()
        self._start_memgraph()

    def __enter__(self):
        return self
    
    def _write_compose_file(self)->Path:
        compose_file_path = Path(self.config_file).parent / "sda-compose.yaml"
        with open(compose_file_path, "w") as f:
            f.write(build_compose_file())
        return compose_file_path
    
    def _start_memgraph(self):
        # Starts the Database first
        try:
            subprocess.run(
            ["docker", "compose", "-f", self.compose_file, "up", "-d"],
            check=True,
            capture_output=True,
            text=True
            )
            return True
        except subprocess.CalledProcessError as e:
            print(f"Error starting memgraph database service:\n", e.stdout, e.stderr)
            return False

    def _mounts(self):
        """
        Returns a list of volume mounts for the Docker container.
        """
        mounts = [
            f"{os.path.abspath(os.path.dirname(self.config_file))}:/data/cfg",
            f"{os.path.abspath(os.path.dirname(self.input_file))}:/data/input",
        ]
        return mounts

    def _build_docker_run_command(self):
        docker_command = [
            "docker", "compose", "-f", self.compose_file, "run", "--rm"
        ]
        for mount in self._mounts():
            docker_command.extend(["-v", mount])
        docker_command.append(SERVICE_NAME)
        docker_command.extend([
            "SettlementDelineationNeighbours",
            "-c", f"/data/cfg/{Path(self.config_file).name}",
            "-i", f"/data/input/{Path(self.input_file).name}"
        ])
        return docker_command
    
    def run(self):
        print(f"Waiting for memgraph database to start...")
        time.sleep(1)  # Wait for the database to be ready
        try:
            print("Running SettlementDelineationNeighbours (Settlement Graph Construction)...")
            result = subprocess.run(self._build_docker_run_command(), check=True, capture_output=True, text=True)
            print(result.stdout)
            if result.stderr:
                print("STDERR:", result.stderr)
            print(f"Settlement Graph Construction completed successfully. Graph is now available in Memgraph. You can access Memgraph Lab at http://localhost:{MEMGRAPH_LAB_PORT}")
        except subprocess.CalledProcessError as e:
            print("Error executing SDA Workflow:\n", e.stdout, e.stderr)

    def __exit__(self, exc_type, exc_value, traceback):
        """
        Clean up resources, if necessary.
        """
        pass
        
def parse_args_to_workflow_object() -> SDAGraphConstruction:
    parser = argparse.ArgumentParser(description="Run SettlementDelineationNeighbours (Settlement Graph Construction) in Docker Compose")
    parser.add_argument("-c", "--config", required=True, help="Path to config file")
    parser.add_argument("-i", "--input", required=True, help="Path to input file")
    args = parser.parse_args()
    return SDAGraphConstruction(
        config_file=args.config,
        input_file=args.input
    )

def get_debug_workflow() -> SDAGraphConstruction:
    return SDAGraphConstruction(
        config_file="/home/lolo/Documents/fishnet/app/sda-workflow/sda-docker.json",
        input_file="/home/lolo/Documents/fishnet/data/samples/Corvara_IT.tiff"
    )

if __name__ == "__main__":
    debug = False
    workflow = get_debug_workflow() if debug else parse_args_to_workflow_object()
    with workflow as workflow:
        workflow.run()