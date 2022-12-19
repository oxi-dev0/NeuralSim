<h1 align="center"> NEURALSIM </h2>
<p align="center">
    <a href="#">
        <img src="https://img.shields.io/github/repo-size/oxi-dev0/NeuroSim" alt="Repo Size">
    </a>
    <a href="#">
        <img src="https://img.shields.io/github/stars/oxi-dev0/NeuroSim" alt="Stars">
    </a>
    <a href="#">
        <img src="https://img.shields.io/github/forks/oxi-dev0/NeuroSim" alt="Forks">
    </a>
    <a href="#">
        <img src="https://img.shields.io/github/watchers/oxi-dev0/NeuroSim" alt="Watchers">
    </a>
</p>

<h3 align="center"> A C++ neural evolution simulator </h3>
<h4 align="center"> This program simulates neural evolution over generations through the goal of survival. </h4>
<h6 align="center"> Please Note: This was inspired by <a href='https://github.com/davidrmiller/biosim4'>David R. Miller's simulator</a>, and is just a quick project i wanted to write; It does not have very optimised / clean code. </h1>
<br>

## Important
#### When you clone this repository, you must use the argument "--recursive" to make sure it pulls all the submodules.

<h3> Branches </h3>

- main > This is the normal branch, just the base program.
- gpu-acceleration-wip > This is where i am working on GPU Acceleration using compute shaders. I do not know if this will work well to speed up simulation.

<h3> Features </h3>

- [x] Generational Simulator
- [x] Neural Map Viewer / Visualiser
- [x] Shape Parsing Language
- [ ] GPU Acceleration
- [ ] Simulation of custom Neural Maps

<h3> Dependencies </h3>
<h6> Note: These are all included in the repo, and all (apart from FFMPEG) are statically linked into the program </h6>

- [Premake](https://github.com/premake/premake-core) - VS Project Generation
- [SFML](https://github.com/SFML/SFML) - Window/Image Rendering
- [FFMPEG](https://github.com/FFmpeg/FFmpeg) - Video Rendering
- [mINI](https://github.com/pulzed/mINI) - INI Config Reading
- [nodesoup](https://github.com/olvb/nodesoup) - Fruchterman-Reingold graph positioning
- [spdlog](https://github.com/gabime/spdlog) - Output logging

<br>

<h2> Program Usage </h2>
<h6> A compiled version of the program can be found in bin/Release-windows-x86_64/ </h6>

<h3> Simulation </h3>

1. Edit `config/config.ini` in the program directory to change the simulation parameters
2. Setup `config/survival.shape` in the program directory to configure the [Survival Conditions](#-survival-conditions--shape-files-)
3. Run `NeuralSim.exe`, optionally with the argument `-c [.INI FILE]`

<h3> Neural Map Visualisation </h3>

1. Run `NeuralSim.exe` with the argument `-v [.NM FILE]`

<h3> Command Line Arguments </h3>

| Argument | Description |
| --- | --- |
| `-h` | Outputs program usage  |
|  |
| `-s` | Simulate mode **[DEFAULT]** |
| `-v [*.nm]` | Visualise mode |
|  |
| `-c [*.ini]` | Specify configuration file **[DEFAULT: `config/config.ini`]** |
| `-sc [*.shape]` | Specify the survival shape file **[DEFAULT: `config/survival.shape`]** |
| `-k [int]` | Set the Fruchterman-Reingold constant **[DEFAULT: 400]** |

> **Please Note**: The square brackets in this documentation only show there is a token or parameter to change, the program will not understand your arguments if you include square brackets when executing the program. Also, the program does not support wildcards (*), this is only in the documentation to show that a specific file of that filetype is required.

<br>

<h2> Source </h2>
<h6> Note: You must have VS2022 installed to build from source </h6>

<h3> Project Setup </h4>

1. Run `GenerateScripts.bat` to use premake to generate the Visual Studio 2022 project files
2. Open NeuralSim.sln

<h3> Receptors </h3>

Receptors are defined in a `Receptor` enum in `src/Classes/NeuralMap/Types.h`  
Receptor behaviour is defined in `Cell::GetReceptorVal(NeuralNet::Receptor receptor)` in `src/Classes/NeuralMap/Receptors.cpp`  
Receptor name strings are defined in `receptorName(Receptor receptor)` in `src/Classes/NeuralMap/DebugTypes.cpp`  

<h3> Effectors </h3>

Effectors are defined in an `Effector` enum in `src/Classes/NeuralMap/Types.h`  
Effector behaviour is defined in `ProcessEffectorQueue(int owner)` in `src/Classes/NeuralMap/Effectors.cpp`  
Effector name strings are defined in `effectorName(Effector effector)` in `src/Classes/NeuralMap/DebugTypes.cpp`

###### The Visualiser long name strings are defined in `shrtNodeToLong(std::string shrt)` in `src/Visualise/Visualise.cpp`

> This section will be expanded along with comments in the source

<br>

<h2> Brief Documentation </h2>
<h6> This documentation will be expanded in the future, and I may make a youtube video explaining how the simulator and visualiser works (as in more technical / code orientated detail than <a href="https://www.youtube.com/watch?v=N3tRFayqVtk">David Miller's video</a>)</h6>
<h3> Survival Conditions & Shape Files </h3>

The `.shape` file is a custom language that allows the program to parse shapes for the simulation. It is extremely basic right now, and is only used for survival conditions. In the future, they will be used to define walls inside the simulation. The language needs specific usage of spaces and newlines to parse correctly.

__Shapes__
| Shape | Arguments | Example |
| --- | --- | --- |
| `CIRCLE` | `[LOC X]` `[LOC Y]` `[RADIUS]` | `CIRCLE 64 64 25` |
| `RECT` | `[LEFT X]` `[RIGHT X]` `[TOP Y]` `[BOTTOM Y]` | `RECT 64 128 0 128` |

Right now, shapes are used to define areas where cells will survive at the end of a generation. In the future, kill shapes will be added, where cells are killed off if they are inside the shape for a certain amount of time. Wall shapes will also be added so that dynamic maps for the cells to survive in can be created.

__Conditions__  
| Keyword | Arguments | Description | Example |
| --- | --- | --- | --- |
| `IF` | `[LEFT VAL]` `[OPERATOR]` `[RIGHT VAL]` | Sets the current condition to parse with | `IF {GEN} > 200` |
| `ELSE` |  | Inverts the current condition | `ELSE` |
| `ENDIF` |  | Clears the current condition | `ENDIF` |  
> Please Note: Conditions do not support expression inputs yet. The left val and right val can only be a Token, or an integer. Expression parsing will come in the future. (This means that `{STEPSPERGEN}/2` would be invalid)

Conditions allow shapes to change during a simulation. They are extremely simple right now and so do not support math operations as inputs. However, they do support all conditional operators (`==`, `!=`, `>`, `>=`, `<`, `<=`).  

__Tokens__
| Token | Description |
| --- | --- |
| `STEP` | The current simulation step |
| `STEPSPERGEN` | The total amount of simulation steps in a generation |
| `GEN` | The current generation |  
> Please Note: Currently, Tokens can only be used in conditions.

Tokens are used for specifying simulation parameters in a shape file. They are used by enclosing the variable name in `{...}`; for instance: `{GEN}`, which returns the simulation's current generation.


Shape condition tokens are not very useful for survival configuration files as changing them during a generation wont do anything; It only matters what shape is active at the end of the generation, meaning only `{GEN}` is useful. I might add random tokens, and I definitely will add shape files for map configuration, so moving walls, etc, can be programmed.

<br>

An example of a `.shape` file is:
###### circle2rect.shape
```
~ Will switch to rectangle after generation 500
IF {GEN} >= 500
  RECT 64 128 0 128	~ Right side box
ELSE
  CIRCLE 64 64 25   	~ Center circle
ENDIF

~ Circle is always present in left corner (ENDIF clears the current condition)
CIRCLE 0 128 25

~ To use this run `NeuralSim.exe -sc config/circle2rect.shape`
```
> `~` can be used to comment either on a full line or after syntax

<br>

<h3> Definitions </h3>
<h4> Receptor Nodes </h4>

Receptors are inputs for neural maps that represent a cell's senses, and other inputs such as an oscillator as an internal clock. Right now there are 5 receptors (`LOC_X`, `LOC_Y`, `RANDOM`, `AGE`, and `OSCILLATOR`).

<h4> Effector Nodes </h4>

Effectors are outputs for neural maps that affect the cell's position. Right now there are 9 movement effectors, and 0 misc effectors. Misc effectors could be used to add pheremones or audio / visual communication between cells.

<h4> Internal Nodes </h4>

Internal nodes are intermediate nodes in neural maps that act as multipliers or variables for it. They allow inputs to be summed up and output into another node. They can also connect to themselves, which act as a sort of data delay, as a loop back connection uses the node's value from the last step.

<h4> Steps </h4>

Steps are an 'update' in the simulation. Each step a cell can move 1 position in any direction as a result of their neural map. Neural maps are simulated every step.

<h4> Generations </h4>

A Generation is a full lifetime of a cell. A generation has a certain number of steps and once a generation has passed, every cell that does not meet the survival conditions is killed off. Then, a new set of cells are spawned, with their Genome being a mutated mix of a random pair of cells that survived the previous generation.

<h4> Gene </h4>

A Gene is a connection between two Nodes in a Neural Map. A source is the node it comes from and a sink is the node it connects to. It has a weight and it multiplies it's source's output by its weight, then passes that to it's sink's input.

<h4> Genome </h4>

A Genome is a list of genes that defines a full neural map. Genomes can be mutilated and combined to form child genomes.

<h4> Neural Map </h4>

A Neural Map is a set of nodes that are connected using the connections described in a Genome. Each cell has a Neural Map, and for every step of the generation, every cell's neural map is simulated to determine the cell's behaviour that step.

<h4> Cell </h4>

A simple "microorganism" that contains a Neural Map, and a location on the grid.  

<br>

<h2> Current State </h2>

<p> The main simulator and visualiser is finished. What's mostly needed is optimisation, and a couple other features that make the program more fun to play around with. </p>

<br>

<h2> Contribution </h2>

<p> The most help is needed with optimisation, as I only have ~1 years experience with C++, and there will be many slow operations and calls that I wrote without realising how slow they are. I also dont have much time to work on the program, so any expansion with new receptors, effectors, etc is greatly appreciated :D </p>

<br>

<h2> Screenshots / Videos </h2>
<h4> 10,000 Gen Simulation with it's Neural Map Visualisation </h4>

https://user-images.githubusercontent.com/33568643/167718427-d59df614-6cee-4036-9e8e-da55312b2032.mov

<img src="https://user-images.githubusercontent.com/33568643/167718510-aed3cfbe-784d-4614-9113-9179fbddad78.png" height="300">

