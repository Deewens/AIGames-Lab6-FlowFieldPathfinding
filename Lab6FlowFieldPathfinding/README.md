## How to use?

- **Left click** to place the goal
- **Middle click** to place the start
- **Right click** to place impassable nodes (walls)

## Troubleshooting

- If the application crashes when you try to place a wall or the start, be sure to place a goal node (left click). That
  is something I need to fix.
- Be sure to adapt the **ScreenSize** constant to your screen resolution. Also, be sure to adapt the size of the Agent (you
  can do that in the constructor in **Agent.cpp**, change the radius and outlineThickness, so the agent is smaller than
  the size of each cells)