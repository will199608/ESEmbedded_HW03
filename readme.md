HW03
===
This is the hw03 sample. Please follow the steps below.

# Build the Sample Program

1. Fork this repo to your own github account.

2. Clone the repo that you just forked.

3. Under the hw03 dir, use:

	* `make` to build.

	* `make clean` to clean the ouput files.

4. Extract `gnu-mcu-eclipse-qemu.zip` into hw03 dir. Under the path of hw03, start emulation with `make qemu`.

	See [Lecture 02 ─ Emulation with QEMU] for more details.

5. The sample is a minimal program for ARM Cortex-M4 devices, which enters `while(1);` after reset. Use gdb to get more details.

	See [ESEmbedded_HW02_Example] for knowing how to do the observation and how to use markdown for taking notes.

# Build Your Own Program

1. Edit main.c.

2. Make and run like the steps above.

3. Please avoid using hardware dependent C Standard library functions like `printf`, `malloc`, etc.

# HW03 Requirements

1. How do C functions pass and return parameters? Please describe the related standard used by the Application Binary Interface (ABI) for the ARM architecture.

2. Modify main.c to observe what you found.

3. You have to state how you designed the observation (code), and how you performed it.

	Just like how you did in HW02.

3. If there are any official data that define the rules, you can also use them as references.

4. Push your repo to your github. (Use .gitignore to exclude the output files like object files or executable files and the qemu bin folder)

[Lecture 02 ─ Emulation with QEMU]: http://www.nc.es.ncku.edu.tw/course/embedded/02/#Emulation-with-QEMU
[ESEmbedded_HW02_Example]: https://github.com/vwxyzjimmy/ESEmbedded_HW02_Example

--------------------

- [ ] **If you volunteer to give the presentation next week, check this.**

--------------------

**★★★ Please take your note here ★★★**
# main.c 設計

從網路上的資料中得知，C的Function在讀取變數時會讀取Register中r0~r3中的值，再進入Function運算  
於是設計一個需要4個變數以下的Function以及一個需要4個變數以上的Function來觀察程式運行流程
<pre><code>
int sub(int a, int b)
{
    return (a-b);
}


// pass multiple paramter
int add(int a, int b, int c, int d, int e, int f)
{
        return a + b + c + d + e + f;
}

void reset_handler(void)
{
        int a = 7, b = 5;
        int c;

                // pass two integers
        c = sub(a, b);

                    // pass six integers
        c = add(1, 2, 3, a, b, c);

        while (1)
         ;
}
</code></pre>

# 觀察
主程式:

00000054 <reset_handler>:  
  54:	b580      	push	{r7, lr}   		r7為指標參數  
  56:	b086      	sub	sp, #24  
  58:	af02      	add	r7, sp, #8  		預留環境變數的記憶體空間  
  5a:	2307      	movs	r3, #7  		環境變數:a  
  5c:	60fb      	str	r3, [r7, #12]  
  5e:	2305      	movs	r3, #5  		環境變數:b  
  60:	60bb      	str	r3, [r7, #8]  
  62:	68f8      	ldr	r0, [r7, #12]  		將a值取出到r0  
  64:	68b9      	ldr	r1, [r7, #8]  		將b值取出到r1  
  66:	f7ff ffcf 	bl	8  			進到副程式sub運算(r0 r1)  
  6a:	6078      	str	r0, [r7, #4]  		r0回傳到預留環境變數空間c  
  6c:	68bb      	ldr	r3, [r7, #8]  		將超過4個以上的輸入變數放到堆疊記憶體  
  6e:	9300      	str	r3, [sp, #0]  
  70:	687b      	ldr	r3, [r7, #4]  
  72:	9301      	str	r3, [sp, #4]  
  74:	2001      	movs	r0, #1  
  76:	2102      	movs	r1, #2  
  78:	2203      	movs	r2, #3  
  7a:	68fb      	ldr	r3, [r7, #12]  
  7c:	f7ff ffd2 	bl	24 <add>  		進到副程式add運算(r0 r1 r2 r3)  
  80:	6078      	str	r0, [r7, #4]  		r0回傳到預留環境變數空間c  
  82:	e7fe      	b.n	82 <reset_handler+0x2e>  
	
副程式sub:  
00000008 :  
   8:	b480      	push	{r7}  
   a:	b083      	sub	sp, #12  
   c:	af00      	add	r7, sp, #0  
   e:	6078      	str	r0, [r7, #4]  
  10:	6039      	str	r1, [r7, #0]  
  12:	687a      	ldr	r2, [r7, #4]  
  14:	683b      	ldr	r3, [r7, #0]  
  16:	1ad3      	subs	r3, r2, r3  
  18:	4618      	mov	r0, r3  		r0為function回傳值  
  1a:	370c      	adds	r7, #12  
  1c:	46bd      	mov	sp, r7  
  1e:	f85d 7b04 	ldr.w	r7, [sp], #4  		指標位址回到主程式位址  
  22:	4770      	bx	lr  

副程式add:  
00000024 :  
  24:	b480      	push	{r7}  
  26:	b085      	sub	sp, #20  
  28:	af00      	add	r7, sp, #0  
  2a:	60f8      	str	r0, [r7, #12]  
  2c:	60b9      	str	r1, [r7, #8]  
  2e:	607a      	str	r2, [r7, #4]  
  30:	603b      	str	r3, [r7, #0]  
  32:	68fa      	ldr	r2, [r7, #12]  
  34:	68bb      	ldr	r3, [r7, #8]  
  36:	441a      	add	r2, r3  
  38:	687b      	ldr	r3, [r7, #4]  
  3a:	441a      	add	r2, r3  
  3c:	683b      	ldr	r3, [r7, #0]  
  3e:	441a      	add	r2, r3  
  40:	69bb      	ldr	r3, [r7, #24]  
  42:	441a      	add	r2, r3  
  44:	69fb      	ldr	r3, [r7, #28]  
  46:	4413      	add	r3, r2  
  48:	4618      	mov	r0, r3  		r0為function回傳值  
  4a:	3714      	adds	r7, #20  
  4c:	46bd      	mov	sp, r7  
  4e:	f85d 7b04 	ldr.w	r7, [sp], #4  		指標位址回到主程式位址  
  52:	4770      	bx	lr  




