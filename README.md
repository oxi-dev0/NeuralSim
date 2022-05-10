<h1 align="center"> NEUROSIM </h2>
<p align="center">
    <a href="#">
        <img src="https://img.shields.io/github/repo-size/oxi-dev0/UE4-BP2CPP" alt="Repo Size">
    </a>
    <a href="#">
        <img src="https://img.shields.io/github/stars/oxi-dev0/UE4-BP2CPP" alt="Stars">
    </a>
    <a href="#">
        <img src="https://img.shields.io/github/forks/oxi-dev0/UE4-BP2CPP" alt="Forks">
    </a>
    <a href="#">
        <img src="https://img.shields.io/github/watchers/oxi-dev0/UE4-BP2CPP" alt="Watchers">
    </a>
</p>

<h3 align="center"> A script to convert Unreal Engine's Blueprints to C++. </h3>
<h4 align="center"> My goal is to generate readable and editable code that can be changed after the conversion, compared to UE4's Nativisation Conversion, which is not very readable. </h4>
<h6 align="center"> Please Note: This is not finished. Check <a href="#-current-state-">here</a> to see its current state. </h1>
<br>

<h3> About </h3>
<p>BP-2-CPP is a script to convert Blueprints into C++. It is meant to make a tedious task a little easier. It is not a perfect solution (it is not even finished yet) and will always have edge cases.</p>

<h3> Prerequisites </h3>
<p>Your project must already be a C++ project, and you must general knowledge of C++ and UE4. You will be required to compile the module once the classes have been generated, and there may be issues due to the way the code is generated.</p>

<br>

<h2> Documentation </h2>

<h3> Usage </h4>

1. First, you need to export the blueprint to a .COPY file. Right click on the asset and go to `Asset Actions > Export...` and save it wherever you like.
2. Next, Run `BP2C++.py`. When prompted, pass in the .COPY file.
3. It should then ask you for your Project's API. This is generated when you convert your project to a C++ project, and typically follows this structure: `[PROJECTNAME]_API`.
4. After these fields have been provided correctly, the script will generate a .h and .cpp file in the directory `[BPNAME]/`. Please place these correctly within your projects source folder, then compile. If the script has worked correctly, you should then be able to move all your references to the new class.
<br>
<h3> Current State </h3>

<p><b>BP-2-CPP is extremely early on. I only started development on it a couple days ago, and currently it can only generate Variable Definitions, with meta data and uproperty params. Eventually, it will be able to generate actual c++ code by tracing along the BP connection paths, using data from the .COPY file. Im not fully sure if I will ever finish this, but I have made a lot of progress, and I hope that it is actually possible. </b></p>

<br>
<h3> Contribution </h3>

<p>BP-2-CPP is still in its early stages, and so more help is needed around parsing the .COPY files to find more data such as variable defaults. Also, I am somewhat new to C++ - around 3-4 months or so - and I do not know all of the specifics, so if any of the code is set to generate wrong, please correct it, and pull request it.</p>
<br>
<p>One thing I do need help on is getting variable types from classes, as .COPY files specify variable types as, for example `VarType=(PinCategory="struct",PinSubCategoryObject=ScriptStruct'"/Script/CoreUObject.Vector"')`, and I do not know how to get the actual c++ class - FVector - from this reference. Thankyou to anyone that helps :)</p>

<br>
<h3> Footnotes </h3>

<p>The documentation will be expanded as I continue to work on the script :D</p>
