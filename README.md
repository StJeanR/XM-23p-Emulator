# XM-23p-Emulator
Emulator for the XM 23p, a theoretical pipeline processor with Harvard architecture and two separate memories, one for instruction data and one for memory. This was created for a Computer Architecture course at Dalhousie University, taught by Dr. Larry Hughes.

To use the emulator;
  1. build it as a project.
  2. navigate to the .exe file's folder and copy over (or make your own) .XME test file(s) from the test folder.
  3. Run the application, and type the name of a copied file, when prompted, to load it.

When the file loads you will see the Main Menu, type the letter(s) of a instruction to run it;

Main Menu

l       -Load File (load a new file)

mi      -View Instrucion Memory

md      -View Data Memory

        format: m_ xxxx xxxx (_ is 'i' or 'd', x is the address to view)

D       -Decode

g       -go  (begin running the program)

r       -change registers or memory  (opens debugger menu, where you can inspect or change memory)

b       -New Break Address  (specify where to add a break to the program at an addres

R       -Reset (load a new program to run)

x       -Exit


Debugger menu;

rd      -Display Register

rc      -Change Register Value

m_      -Change Memory at Address in instruction (_i) or data (_d) memory

b       -Set Break Address

p       -Display PSW

x       -Return  (back to main menu)
