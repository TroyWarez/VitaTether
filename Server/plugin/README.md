# noled
noled is a simple plugin to turn off the blue home button led of PS Vita. Especially useful at night. With the release of v1.2, it will now disable both the Home Button LED and Game Cartridge LED.

https://youtu.be/YbxpdJ67VgE

It is a kernel plugin so enable it in kernel section of your config.txt.

copy noled.skprx to tai folder (ux0:/tai)
edit config.txt:

*KERNEL

ux0:tai/noled.skprx

after that, reboot your vita and restart henkaku.

All credits to @xerpi, all i did was to compile the source.

A known issue is, after resuming from standby, blue light will turn on for a second and then turn off.
