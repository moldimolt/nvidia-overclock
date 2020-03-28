# nvidia-overclock
This repository is a mirror of https://1vwjbxf1wko0yhnr.wordpress.com/2015/08/10/overclocking-tools-for-nvidia-gpus-suck-i-made-my-own/. The site hasn't been updated in over 4 years so I'm mirroring it so the code isn't lost if it goes down. Archived version available [here](https://web.archive.org/web/20180221193221/https://1vwjbxf1wko0yhnr.wordpress.com/2015/08/10/overclocking-tools-for-nvidia-gpus-suck-i-made-my-own/).

The usage syntax is `overclock.exe GPU_CLOCK VRAM_CLOCK` i.e. `overclock.exe 135 250` for a 135MHz GPU overclock and 250MHz GPU VRAM overclock.

# Known Bugs 
1. Doubled Memory OC reading: When running overclock.exe with no arguments after overclocking, the memory OC reading will appear doubled, e.g. a memory OC of 250MHz (`overclock.exe 135 250`) will show `Current RAM OC: 500MHz`. Solely visual and doesn't effect functionality.
1. Inaccurate GPU/memory clock reading: On GPUs past the 9xx series, overclock.exe reads garbage values for the current GPU/GPU memory clock. Solely visual and doesn't effect functionality.
1. Lacks GDDR6 memory support: Having not been updated since 2015, nvidia-overclock doesn't account for the existence of GDDR6 (i.e. 2xxx series GPUs) and using it on these GPUs causes buggy, potentially dangerous behavior.

# License 
```
            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.
```
