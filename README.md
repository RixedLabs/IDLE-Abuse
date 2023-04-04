# IDLE-Abuse

<img title="cat-is-cute" alt="Alt text" src="https://github.com/RixedLabs/IDLE-Abuse/blob/main/random-cat.gif">

IDLE Abuse :A simple technique to detect when a Windows process becomes idle and inject malicious code into it. This technique relies on an undocumented Windows API function called RegisterWaitForInputIdle, which allows a callback function to be registered and called when a process becomes idle.

The PoC, written by [Navneet Raj](https://twitter.com/_muffin31) and [ElementalX](https://twitter.com/ElementalX2) , demonstrates the use of this technique by registering a callback function that injects a meterpreter shellcode into the Windows calculator process when the process spawned by `WinExec` function becomes idle. Thanks to folks at Stack Overflow & REACT OS Source code for showing us the correct direction. 


Special thanks to [modexp](https://twitter.com/modexpblog) for helping us with the code overview. 
