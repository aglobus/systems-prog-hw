/* Assignment #5 Alex Globus */
/* B&O 5.16, 5.18, 5.22 */
/* B&O 6.26, 6.28, 6.30, 6.31, 6.32, 6.35, 6.36, 6.37, 6.45 */

/*

5.16

Write a version of the inner product procedure described in Problem 5.15 that uses four-way loop unrolling.
For x86-64, our measurements of the unrolled version give a CPE of 2.00 for integer data but still 3.00 for both single and double precision.

*/


void inner4(vec_ptr u, vec_ptr v, data_t *dest)
{
  long int i;
  int length = vec_length(u);
  data_t *udata = get_vec_start(u);
  data_t *vdata = get_vec_start(v);
  data_t sum = (data_t) 0;

  /* original
  for (i = 0; i < length; i++) {
    sum = sum + udata[i] * vdata[i];
  }

          .L87:

          movss (%rbx,%rdx,4), %xmm0      ; Get udata[i]
          mulss (%rax,%rdx,4), %xmm0      ; Multiply by vdata[i]
          addss %xmm0, %xmm1              ; Add to sum
          addq $1, %rdx                   ; increment i
          cmpq %rc, %rdx                  ; compare i with length
          jl .L87                         ; break if <


  */
  
  /* 4 way loop unroll */
  for (i = 0; i < length; i+=4) {
    sum = sum + udata[i] * vdata[i];
    sum = sum + udata[i+1] * vdata[i+1];
    sum = sum + udata[i+2] * vdata[i+2];
    sum = sum + udata[i+3] * vdata[i+3];
  }

  /* A. Explain why any version of any inner product procedure cannot achieve a CPE less than 2.00. */
  
  /*
    A CPE of 2.00 is the lower bound in this case, since there are two critical paths and sum is dependent on both udata and vdata to update itself.

  */

  /* B. Explain why the performance for floating-point data did not improve with loop unrolling */

  /*
    Even though the loop has been unrolled, there are still n multiply operations along the critical path. Additionally, the compiler does not perform any reassociation transformation to types of floating point data.
  */

  *dest = sum;
}


/*
 5.18

Write a version of the inner product procedure described in Problem 5.15 that uses four-way loop unrolling along with reassociation to enable greater parallelism. Our measurements for this function give a CPE of 2.00 with x86-64 and 2.25 with IA32 for all types of data.

*/

void inner4(vec_ptr u, vec_ptr v, data_t *dest)
{
  long int i;
  int length = vec_length(u);
  data_t *udata = get_vec_start(u);
  data_t *vdata = get_vec_start(v);

  data_t sum = (data_t) 0;
  data tmp1, tmp2, tmp3;

  /* 4 way loop unroll */
  for (i = 0; i < length; i+=4) {
    sum = sum + udata[i] * vdata[i];
    tmp1 = tmp1 + udata[i+1] * vdata[i+1];
    tmp2 = tmp2 + udata[i+2] * vdata[i+2];
    tmp3 = tmp3 + udata[i+3] * vdata[i+3];
  }

  /* do the remaining */
  for (; i < length; i++)
    sum += udata[i] * vdata[i];

  sum += tmp1 + tmp2 + tmp3;

  *dest = sum;
}


/*
 5.22

 Suppose you are given the task of improving the performance of a program consisting of three parts. Part A requires 20% of the overall run time, part B requires 30%, and part C requires 50%. You determine that for $1000 you could either speed up part B by a factor of 3.0 or part C by a factor of 1.5. Which choice would maximize performance?

Speeding up B by a factor of 3 will yield a higher performace gain. Let T represent the total runtime as a sum of the three parts:

T = A + B + C

  = 0.2T + 0.3T + 0.5T

We can apply speed up B by a factor of 3 or C by a factor of 1.5, in other words, B will finish at a 1/3 of its original speed or C will finish at 2/3 of its original runtime. So the question becomes:

A + (1/3)B + C < A + B + (2/3)C

If the above statement is true, then B is the faster optimization. Otherwise, C is faster. Substitute and solve:

0.2T + 1/3(0.3T) + 0.5T < 0.2T + 0.3T + 2/3(0.5T)

0.8T < 0.5T + 1/3T

4/5T < 5/6T

The last statement is true (4/5 is less than 5.6), meaning that B is the faster optimization.

*/


/*
 6.26
The following table gives the parameters for a number of different caches. For each cache, fill in the missing fields in the table. Recall that m is the number of physical address bits, C is the cache size (number of data bytes), B is the block size in bytes, E is the associativity, S is the number of cache sets, t is the number of tag bits, s is the number of set index bits, and b is the number of block offset bits. Cache   m      C       B     E      S    t     s     b
 1       32    1024     4     4     64    24    6     2
 2       32    1024     4    256     1    30    0     2
 3       32    1024     8     1     128   22    7     3
 4       32    1024     8    128     1    29    0     3
 5       32    1024     32    1     32    22    5     5
 6       32    1024     32    4      8    24    3     5

 */


/*
 6.28
 This problem concerns the cache in Problem 6.13.
A. List all of the hex memory addresses that will hit in set 1.

Two way associative w/E = 2, B = 4, S = 8
13 bit wide address

CT = 8bits
CI = 3bits
CO = 2bits

Line 0: 0100 0101 001 XX
          0x45    0x1 offset

Concatenated -> 0x8A4 (0 0)
                0x8A5 (0 1)
                0x8A6 (1 0)
                0x8A7 (1 1)

Conversely for Line 1: 0x0011 1000 001 XX

0x704, 0x705, 0x706, 0x707

B. List all of the hex memory addresses that will hit in set 6.

0x91 concat 0x6 -> 1001 0001 110 XX

0x1238, 0x1239, 0x123A, 0x123B
*/

/*
 6.30
A.

 11  10  9   8   7   6   5   4   3   2   1   0
 CT  CT  CT  CT  CT  CT  CT  CT  CI  CI  C0  C0

B.

 0x834 Hits -> 44

 0x836 Hits -> 45

 0xFFD Hits -> 9A

*/

/*
 6.31

A.
  C = B x E x S
    = 4 x 4 x 8
    = 128

B.

12  11  10  9   8   7   6   5   4   3   2   1   0
CT  CT  CT  CT  CT  CT  CT  CT  CI  CI  CI  C0  C0

*/

/*
 6.32

 0x71A

 0 0111 000 110 10

 0x38   0x6   0x2

 - Valid bit not set

 
 */


/*
 6.35

 dst array

    0   1   2   3
0   m   m   m   m
1   m   m   m   m
2   m   m   m   m
3   m   m   m   m

src array

    0   1   2   3
0   m   m   h   m
1   m   h   m   h
2   m   m   h   m
3   m   h   m   h


*/


/*
 6.37

  int x[2][128];
  int i;
  int sum = 0;

  for (i = 0; i < 128; i++) {
    sum += x[0][i] * x[1][i];
  }

A. Case 1: Assume the cache is 512 bytes, direct-mapped, with 16-byte cache blocks. What is the miss rate?
  
  There will be, at a minimum, at least one miss for every four ints. Given 2*128 = 256, and that there are 64 blocks blocks of four ints, 64/256 = 25% miss rate

B. Case 2: What is the miss rate if we double the cache size to 1024 bytes?

  No change, the miss rate should still be 25% as long as the block size and design (direct mapped) are the same.

Case 3: Now assume the cache is 512 bytes, two-way set associative using an LRU replacement policy, with 16-byte cache blocks. What is the cache miss rate?

  Less than 25%
  
For Case 3, will a larger cache size help to reduce the miss rate?

No. It doesn't affect the miss rate.

For Case 3, will a larger block size help to reduce the miss rate?

Yes. Larger blocks means the cache will hit more often (if the entire row is loaded), meaning less misses.

 */


/*
 6.45
Download the mountain program from the CS:APP2 Web site and run it on your favorite PC/Linux system. Use the results to estimate the sizes of the caches on your system.
*/

/*
merlin% ./mountain 
Clock frequency is approx. 2294.9 MHz
Memory mountain (MB/sec)
	s1	s2	s3	s4	s5	s6	s7	s8	s9	s10	s11	s12	s13	s14	s15	s16	s17	s18	s19	s20	s21	s22	s23	s24	s25	s26	s27	s28	s29	s30	s31	s32	s33	s34	s35	s36	s37	s38	s39	s40	s41	s42	s43	s44	s45	s46	s47	s48	s49	s50	s51	s52	s53	s54	s55	s56	s57	s58	s59	s60	s61	s62	s63	s64	
32m	6979.2	6297.6	4990.4	3798.6	2943.7	2516.4	2063.8	1746.5	1661.0	1552.5	1448.0	1382.0	1280.5	1221.5	1212.5	1191.1	1169.2	1140.6	1107.7	1115.0	1086.8	1048.2	1088.4	1098.0	1074.1	1076.7	1112.9	1104.1	1111.2	1114.8	1146.4	985.3	1152.0	1126.2	1146.2	1164.2	1160.1	1153.9	1155.4	1209.7	1231.1	1261.0	1333.9	1331.1	1304.1	1387.3	1388.1	1201.9	1439.8	1453.1	1488.8	1472.2	1497.8	1464.6	1537.1	1492.7	1568.5	1504.9	1621.7	1618.8	1634.8	1640.0	1637.7	1055.1	
16m	7040.6	6491.1	4994.2	3830.6	3127.7	2570.1	2107.7	1837.3	1716.9	1579.4	1471.7	1375.6	1350.4	1348.0	1311.4	1220.3	1224.4	1229.7	1265.0	1245.4	1294.6	1280.2	1315.9	1360.4	1375.5	1318.5	1376.0	1444.2	1480.9	1245.8	1534.8	1099.0	1484.6	1653.8	1604.0	1714.3	1564.6	1747.6	1536.7	1822.0	1883.5	1899.4	1932.1	2081.0	2095.2	2150.2	2042.2	1461.0	2321.2	2192.5	2275.9	2465.8	2570.9	2498.7	2658.1	2122.7	2345.9	2457.0	2727.8	2739.5	2506.1	2345.0	2514.1	1027.8	
8m	6495.1	6512.1	5447.5	4129.4	3356.5	2682.1	2322.3	2004.7	1760.5	1746.0	1652.2	1639.7	1513.1	1517.8	1580.3	1424.8	1786.1	1813.0	1888.5	1925.0	1902.9	2026.1	2138.6	2137.9	2246.7	2226.4	1647.7	2119.2	2183.8	2693.0	2126.3	1238.5	1955.2	2337.1	2226.9	3221.1	2107.1	2722.8	3452.2	2657.6	3140.4	3418.1	2768.3	2890.6	3046.5	3435.3	3540.9	2390.7	3536.3	3818.9	3390.3	3948.4	4063.9	3991.9	4051.1	4139.6	3832.1	3758.0	3784.3	3798.0	3709.2	4446.4	4001.2	1360.9	
4m	7035.5	6055.2	5513.8	4589.5	3688.3	3260.6	2703.7	2397.2	2442.8	2416.8	2350.0	2381.3	2336.3	2679.6	2444.8	2045.1	3206.3	2821.2	2818.1	3011.8	3624.9	3630.6	3794.9	3882.2	3678.4	3832.7	3602.7	3852.5	2421.8	3536.1	3899.3	1817.7	3815.6	3779.8	3786.9	3833.4	4082.9	3990.6	4225.5	3792.4	3757.8	3741.4	3610.3	3799.6	3709.4	4224.8	3877.9	4067.5	3826.3	3821.6	3837.8	4110.5	4073.8	4124.6	3957.0	4064.1	3906.1	4445.3	4110.1	4116.1	4454.5	3770.9	4076.6	1093.4	
2m	6860.3	6714.2	6540.3	6242.4	5986.4	5195.8	4192.2	4209.9	4255.4	4261.0	4213.6	4285.8	4512.0	4500.3	4496.5	4179.4	4082.0	3802.4	4047.9	4155.2	3834.7	4079.3	3932.2	4109.9	4127.4	4056.1	4104.5	4048.5	3830.3	3992.0	4047.3	3869.2	4116.0	4124.8	4123.4	3933.0	4135.1	4006.1	4101.9	4433.6	4384.5	4413.3	4398.7	4352.5	4479.7	4575.1	4361.6	4464.3	4615.3	4346.1	4037.7	4804.0	4645.4	4836.8	4743.0	4661.3	4983.3	5617.5	5608.0	6296.4	6095.1	6515.3	5791.6	2980.7	
1024k	6911.9	6889.3	6888.2	6910.6	6644.1	5644.1	4886.0	4606.5	4268.6	4256.4	4239.5	4233.4	4552.0	4220.5	4210.1	4215.0	4236.9	4204.2	4205.9	4209.2	4201.6	4182.2	4177.2	4161.1	4177.1	4197.5	4207.7	4213.0	4267.6	4349.1	4436.9	4156.2	4742.4	4937.8	5026.8	6794.3	6725.4	6855.5	6644.4	6854.0	6832.5	6845.9	6860.4	6861.9	6805.7	6791.1	6873.2	4264.7	6867.3	6846.7	6850.9	6830.1	6863.4	6860.9	6869.5	6845.5	6865.6	6864.1	6854.5	6868.5	6867.4	6866.7	6729.3	4481.4	
512k	6915.9	7445.2	6102.9	6904.0	6652.1	5653.6	4876.3	4294.1	4270.0	4240.0	4238.5	4228.5	4287.5	4291.0	4526.8	4218.2	5814.9	6215.0	6389.3	6510.6	6714.4	6768.4	6774.3	6763.5	6852.6	6874.7	6851.4	6855.3	6852.8	6872.0	6852.2	4191.6	6855.7	6856.6	6712.5	6854.0	6854.1	6856.3	6732.8	6859.5	6845.0	6843.5	6849.2	6842.7	6832.4	6845.0	6842.9	6816.6	6837.3	6847.4	6826.2	6841.2	6841.7	6849.3	6833.7	6835.7	6828.9	6832.4	6833.8	6841.5	6819.5	6818.5	6738.8	4462.3	
256k	6914.2	6910.8	6904.3	6893.9	6865.0	6871.1	6850.6	6667.1	6713.1	6706.5	6840.4	6670.0	6623.8	6609.6	6655.3	6539.0	6661.7	6710.9	6647.5	6604.9	6648.1	6733.2	6653.4	6766.9	6700.5	6683.1	6815.8	6749.8	6737.0	6769.9	6804.1	6796.7	6816.8	6828.8	6793.0	6811.6	6813.4	6799.8	6776.7	6792.3	6785.9	6749.4	6758.2	6764.2	6758.1	6768.2	6721.9	6799.9	6785.6	6788.8	6783.0	6761.2	6756.2	6763.4	6726.5	6735.0	6761.1	6751.6	6788.3	6742.6	6699.1	6761.5	6786.7	6680.7	
128k	6913.2	6804.6	6899.0	6897.6	6898.0	6880.5	6878.4	6876.2	6870.9	6868.9	6865.0	6863.2	6853.3	6848.8	6832.2	6841.2	6836.7	6808.9	6829.2	6792.3	6807.2	6784.4	6789.3	6799.9	6800.3	6785.0	6763.4	6777.5	6791.4	6769.9	6775.6	6767.3	6785.5	6805.2	6765.3	6758.3	6779.3	6776.5	6740.2	6773.0	6778.6	6748.3	6751.7	6753.1	6770.7	6726.4	6706.6	6700.5	6713.9	6743.1	6748.1	6685.6	6732.2	6702.3	6734.8	6737.8	6695.3	6680.5	6733.0	6700.5	6697.5	6738.0	6648.1	6638.3	
64k	6906.5	6895.8	6886.2	6876.2	6862.6	6732.3	6822.7	6833.7	6820.0	6810.8	6794.5	6799.9	6785.0	6777.5	6724.5	6767.3	6805.2	6774.8	6776.5	6754.7	6748.3	6753.1	6726.4	6743.8	6743.1	6763.8	6702.3	6712.5	6680.5	6700.5	6710.0	6666.5	6659.8	6710.5	6639.9	6682.7	6630.0	6616.0	6579.1	6629.6	6572.0	6520.9	6488.6	6596.6	6564.5	6535.8	6609.6	6636.6	6542.1	6578.3	6538.6	6647.2	6611.8	6428.8	6554.4	6485.5	6625.1	6559.3	6599.2	6543.1	6537.6	6485.8	6485.5	6334.1	
32k	6857.4	6769.7	6721.5	6692.6	6584.2	6537.4	6570.2	6605.6	6693.4	6736.5	6693.5	6526.0	6549.3	6509.1	6458.7	6554.9	6551.4	6546.5	6517.9	6594.7	6381.4	6521.1	6458.3	6472.1	6451.2	6468.8	6534.4	6485.5	6445.0	6543.1	6485.8	6619.6	6522.9	6443.0	6431.1	6425.6	6366.6	6464.6	6425.6	6458.7	6215.6	6347.5	6267.7	6418.7	6352.3	6189.2	6297.3	6310.9	6334.6	6369.2	6242.8	6205.0	6166.2	6126.4	6095.7	6074.6	6242.9	6222.6	6212.9	6203.0	6100.7	6088.9	6088.4	6087.9	
16k	6880.0	6826.3	6763.2	6796.7	6773.0	6657.8	6712.5	6695.0	6984.4	6948.2	6903.1	6854.5	6884.6	6662.5	6664.9	6811.4	6676.6	6546.5	6616.0	6414.7	6679.1	6467.4	6535.8	6260.4	6289.3	6541.9	6126.4	6332.5	6504.0	6328.3	6119.6	6283.3	6086.9	6098.5	6033.7	6290.1	7093.2	6593.9	6425.6	6257.3	6271.6	6347.5	6201.0	6186.2	6187.6	6189.2	6050.1	6161.8	6181.3	5558.7	7754.2	7609.3	7235.9	4931.5	7408.4	5076.5	9979.2	10609.1	10421.0	7458.3	7151.4	10445.6	10467.8	5816.7	
8k	7340.7	7230.6	7143.7	7131.8	6447.7	6265.0	6302.8	6283.3	6252.5	6328.3	6119.6	6235.5	5974.2	5617.1	6392.8	5731.6	6098.5	5495.6	5556.7	5185.6	6485.5	6323.6	6865.3	7313.5	7429.9	7157.3	9933.5	10001.5	5578.9	9789.0	9931.9	10129.1	9329.9	9496.0	5316.8	9471.5	9753.2	7475.7	6984.4	6784.8	8782.3	9728.2	9478.8	9279.2	9713.2	10212.2	9982.7	9515.3	9581.1	9351.6	8955.6	9007.4	8619.7	8663.1	7383.5	7283.7	8869.3	9245.0	6884.6	6369.4	6685.0	6585.3	6088.4	7730.1	
4k	7247.3	7014.8	6884.6	6871.2	6526.0	5796.7	5993.3	6564.1	5410.2	5903.1	5928.4	5670.6	6285.9	7529.2	7458.3	7252.9	6635.8	6678.6	7150.7	7431.0	7336.0	7488.5	7427.0	7961.8	7633.9	7832.5	8451.8	8172.0	7525.0	7258.2	7388.3	6831.2	6617.8	5986.6	5836.9	5292.2	5870.6	5338.1	5603.7	6502.1	4939.0	4542.9	5070.1	6467.4	4856.6	5106.1	6050.1	7224.6	5952.3	5808.9	4831.3	5114.3	5522.0	5378.6	4589.7	4786.4	5091.7	5354.7	4798.4	3629.1	3843.9	3786.5	4972.2	2532.3	
2k	6610.2	6283.3	6019.6	5904.4	6385.0	5114.7	7283.7	6385.7	7441.9	5443.6	4639.6	6726.3	7352.9	5235.2	6369.4	5994.8	5620.1	5637.4	5011.2	6502.1	5176.8	4104.3	3521.4	3751.2	4647.1	3086.2	4002.7	3221.6	3492.2	4217.6	3522.4	3192.9	4742.7	2810.0	2893.5	2471.4	1639.2	2828.6	3977.8	2544.3	3748.3	3671.8	2508.3	3518.8	3442.3	2348.2	3289.3	2677.3	3484.8	3059.8	1873.4	3729.2	2180.1	3144.8	3537.9	2232.8	2170.8	2974.8	3251.1	2108.8	3155.4	3155.4	3496.9	4079.8	


Reformat into 3 cols of x y height, pipe into mathematica for fancy graph

ListPlot3D[Import["/path/to/file", "Memory Mountain"]]

Ideally, if I had mathematica or knew how to use gnuplot, I'd have a 3d plot to analyze, but I don't, and this is due in a few hours. Does anybody even read these?

*/
