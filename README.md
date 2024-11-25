# Delay-Tolerant Networking (DTN) Simulation

This project implements a **Delay-Tolerant Networking (DTN) protocol** for data collection in mobile and stationary wireless sensor networks. The simulation is written in C and uses **multi-threading** to represent sensor nodes and base stations, mimicking real-world DTN behavior.

The protocol maximizes the delivery of data packets to their destinations while minimizing power consumption and buffer usage. The system simulates node mobility, packet generation, transmission, and storage, with configurable parameters such as transmission range, buffer size, and mobility speed.

---

## Features

- **Multi-threaded Simulation**:
  - Each node (sensor or base station) is represented by a thread.
  - Nodes communicate via TCP connections.

- **Mobility Modeling**:
  - Mobile nodes move within a square region (0,0) to (1000,1000).
  - Randomized movement directions and speeds, configurable per node.

- **Packet Handling**:
  - Data packets are generated probabilistically and buffered locally.
  - Packets are exchanged between nodes within a configurable transmission range.

- **Epidemic Protocol Simulation**:
  - Nodes opportunistically copy packets to nearby nodes.
  - Packets are uploaded to the destination node when in range.

- **Resource Constraints**:
  - Limited buffer size per node.
  - Energy-efficient transmission policies.

- **Customizable Parameters**:
  - Configurable via command-line arguments, such as:
    - Number of nodes (`N`)
    - Simulation time steps (`K`)
    - Transmission range
    - Node buffer size (`B`)
    - Node mobility speed
    - Probability distribution for packet generation

- **Metrics for Evaluation**:
  - Fraction of successfully delivered packets.
  - Total number of transmissions.

---

## How It Works

1. **Initialization**:
   - Nodes are assigned random start locations.
   - Each node establishes TCP connections with other nodes.

2. **Simulation Loop**:
   - For each time step:
     - Nodes generate packets based on a probability distribution.
     - Mobile nodes move randomly within the simulation region.
     - Nodes exchange position information and determine if they are within transmission range.

3. **Packet Transfer**:
   - If a node is within range of a destination:
     - Upload and delete packets intended for that destination.
   - Otherwise, opportunistically copy packets to nearby nodes, considering buffer space and transmission cost.

4. **Termination**:
   - After `K` time steps, the simulation ends, and metrics are reported.

---

## Command-Line Arguments

The program accepts the following parameters via the command line:

| **Parameter**            | **Description**                                                   |
|--------------------------|-------------------------------------------------------------------|
| `-N <number>`            | Total number of nodes in the simulation.                         |
| `-D <number>`            | Number of destination nodes.                                     |
| `-K <number>`            | Number of simulation time steps.                                 |
| `-B <size>`              | Buffer size per node.                                            |
| `-R <range>`             | Transmission range between nodes.                                |
| `-S <speed>`             | Mobility speed of nodes (randomized per node).                   |
| `-P <distribution>`      | Probability distribution for packet generation (e.g., uniform).  |

---

## Build and Run

### Clone the Repository
```bash
git clone https://github.com/devamp05/DTNSimulator.git
cd DTNSimulator
```

### Compile the Code
Use the provided Makefile to compile the program or to skip compilation and directly used the compiled file skip to Run the Simulation step:

```bash
make
```
### Run the Simulation
Execute the program with your desired parameters:

```bash
./DTN-sim 11 1 1 200 10 20
```

### Output Metrics
1. Packet Delivery Rate:
    - Fraction of data packets successfully delivered to their destinations (Total packets received/ Total hops).
2. Transmission Efficiency:
    - Total number of transmissions during the simulation (Total hops).
3. Additional debug and verification output.

### Implementation Details
 - Threading:
    - Each node runs as a separate thread for parallel simulation.
    - TCP connections simulate real-time communication between nodes.
 - Epidemic Protocol:
    - Packets are copied based on proximity and buffer availability.
    - Policies minimize redundant transmissions and conserve energy.
 - Buffer Management:
    - Nodes drop packets if the buffer becomes full, prioritizing recent data.
 - Future Improvements
    - Implement advanced routing protocols to reduce redundant transmissions further.
    - Incorporate realistic wireless communication models.
    - Introduce energy-based constraints for better resource simulation.
