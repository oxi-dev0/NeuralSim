<h1 align="center"> NEUROSIM </h2>
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

<h3> Features </h3>

- [x] Generational Simulator
- [x] Neural Map Viewer / Visualiser
- [ ] Survival Condition Language
- [ ] Simulation of custom Neural Maps

<br>

<h2> Program Usage </h2>
<h6> A compiled version of the program can be found in bin/Release-windows-x86_64/ </h6>

<h3> Simulation </h3>

1. Edit `config/config.ini` in the program directory to change the simulation parameters
2. Run `BioSim.exe`, optionally with the argument `-c [.INI FILE]`

<h3> Neural Map Visualisation </h3>

1. Run `BioSim.exe` with the argument `-v [.NM FILE]`

<h3> Command Line Arguments </h3>

| Argument | Description |
| --- | --- |
| `-h` | Outputs program usage  |
|  |
| `-s` | Simulate mode **[DEFAULT]** |
| `-v [*.nm]` | Visualise mode |
|  |
| `-c [*.ini]` | Specify configuration file **[DEFAULT: `config/config.ini`]** |

<br>

<h2> Source </h2>
<h6> Note: You must have VS2022 installed to build from source </h6>

<h3> Project Setup </h4>

1. Run `GenerateScripts.bat` to use premake to generate the Visual Studio 2022 project files
2. Open NeuralSim.sln
<br>

<h3> Brief Documentation </h3>
<h6> This documentation will be expanded in the future, and I may make a youtube video explaining how the simulator and visualiser works </h6>
<h5> Survival Condition </h5>

The survival condition is currently determined using `bool HasSurvived(Cell cell)` in `src/Classes/Generation.cpp`. This function is also used to colour the cells in the mp4 generation render (`Output.cpp` - `RenderFrame()`).
In the future, this will be expanded to use a custom programming language that can be written/generated and specified by the program user.

<h5> Receptors </h5>

Receptors are defined in a `Receptor` enum in `src/Classes/NeuralMap/Types.h`  
Receptor behaviour is defined in `Cell::GetReceptorVal(NeuralNet::Receptor receptor)` in `src/Classes/NeuralMap/Receptors.cpp`  
Receptor name strings are defined in `receptorName(Receptor receptor)` in `src/Classes/NeuralMap/DebugTypes.cpp`  

<h5> Effectors </h5>

Effectors are defined in an `Effector` enum in `src/Classes/NeuralMap/Types.h`  
Effector behaviour is defined in `ProcessEffectorQueue(int owner)` in `src/Classes/NeuralMap/Effectors.cpp`  
Effector name strings are defined in `effectorName(Effector effector)` in `src/Classes/NeuralMap/DebugTypes.cpp`  

###### The Visualiser long name strings are defined in `shrtNodeToLong(std::string shrt)` in `src/Visualise/Visualise.cpp`  

<br>

<h3> Current State </h3>

<p> The main simulator and visualiser is finished. What's mostly needed is optimisation, and a couple other features that make the program more fun to play around with. </p>

<br>

<h3> Contribution </h3>

<p> Most help is needed with optimisation, as I only have ~1 years experience with C++, and there will be many slow operations and calls that I wrote without realising how slow they are. I also dont have much time to work on the program, so any expansion with new receptors, effectors, etc is greatly appreciated :D </p>

<br>

<h3> Screenshots </h3>
<h4> 10,000 Gen Simulation with it's Neural Map Visualisation </h4>
<p float="left">
<img src="https://user-images.githubusercontent.com/33568643/167692593-8c4ff5fb-fc6c-4a88-b18e-16d1a6c4013d.png" height="300">
<img src="https://user-images.githubusercontent.com/33568643/167692784-acd1fe3a-903b-4052-99f3-e622ca998cca.png" height="300">
    </p>

