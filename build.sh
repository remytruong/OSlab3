echo Building...

~/toolchains/gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-gcc -mcpu=cortex-a7	-fpic -ffreestanding --specs=nosys.specs -std=gnu99 -T linker.ld src/hal/hal_io_asm.s src/boot.S src/drivers/smartstart/rpi-SmartStart.s src/drivers/stdio/emb-stdio.c src/drivers/sdcard/SDCard.c src/drivers/smartstart/rpi-SmartStart.c src/hal/hal_io.c src/kernel.c src/hal/fonts.c -o output/kernel.elf -O0	-lc -lm -lg -lgcc
~/toolchains/gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-objcopy kernel.elf -O binary kernel.img

echo Success