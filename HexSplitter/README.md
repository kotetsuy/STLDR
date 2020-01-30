# HexSplitter

## Abstruct

This is Split hex file to 128Kbytes. It is necessary for external loader of STM32F412.
In my research, STM32CubeProgrammer download all the write data to STM32 RAM.
If the size of the write data is over STM32 RAM, the writing result is incorrect.

## How to run

```
> python hexsplitter.py < input.hex
```

Please write the splitted hex file on the external memory.