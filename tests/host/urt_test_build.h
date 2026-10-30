/* V5.3.0 R11 (game build 1090142): host tests run on either known witcher3.exe build.
 * run_host_tests.sh sets URT_TEST_BUILD from the executable's SHA-256 (0 = Patch 5.01 build 1048522, 1 = build 1090142).
 * RV(x) = the address of a Patch 5.01 RVA on the build under test: production addresses through urt_rva()
 * (src/urt_build_table.h), plus the addresses only these tests read (engine variables of retired rows, SHaRC
 * shader descriptors, the retired PT light-cap / culling code), found the same way (tools/port/). */
#ifndef URT_TEST_BUILD_H
#define URT_TEST_BUILD_H
#include "../../src/urt_build.h"
#include <stdlib.h>
#include <stdio.h>
static const uint32_t kTestOnly1090142[][2]={
  {0x1c20018u,0x1c1fbc8u},{0x1c20019u,0x1c1fbc9u},{0x1cf2a6du,0x1cf257du},{0x1cf2a84u,0x1cf2594u},{0x1cf2abbu,0x1cf25cbu},{0x1cf2aceu,0x1cf25deu},
  {0x1ee6df0u,0x1ee6700u},{0x1ee6e61u,0x1ee6771u},{0x1eefcd0u,0x1eef5e0u},{0x1ef0010u,0x1eef920u},{0x1ef0fbbu,0x1ef08cbu},{0x1ef26c2u,0x1ef1fd2u},
  {0x1ef2de4u,0x1ef26f4u},{0x1ef2f5du,0x1ef286du},{0x1ef32bdu,0x1ef2bcdu},{0x1ef341eu,0x1ef2d2eu},
  {0x3227270u,0x32257e0u},{0x3227720u,0x3225c90u},{0x32277c0u,0x3225d30u},{0x3227810u,0x3225d80u},{0x3227860u,0x3225dd0u},{0x32278b0u,0x3225e20u},
  {0x33e9ef0u,0x33e7f90u},{0x33e9f40u,0x33e7fe0u},{0x33e9f90u,0x33e8030u},{0x33e9fe0u,0x33e8080u},
  {0x4017c5cu,0x4015c4cu},{0x4067290u,0x40652b0u},{0x40673e0u,0x4065400u},{0x4067418u,0x4065438u},{0x4068528u,0x4066548u},{0x4069588u,0x40675a8u},
  {0x4069ba8u,0x4067bc8u},{0x4069c88u,0x4067ca8u},{0x4069e80u,0x4067ea0u},{0x4069f98u,0x4067fb8u},{0x4069fd0u,0x4067ff0u},{0x406a0b0u,0x40680d0u},
  {0x406a0e8u,0x4068108u},{0x406a9e0u,0x4068a00u},{0x406aac0u,0x4068ae0u},{0x406c858u,0x406a868u},{0x514a208u,0x5148258u},{0x514d280u,0x514b2d0u},
  {0x514df30u,0x514bf80u},{0x514f628u,0x514d678u}};
static int urt_test_build_init(uint32_t image_size){
  const char *e=getenv("URT_TEST_BUILD");
  urt_build_set(e&&e[0]=='1'?URT_BUILD_1090142:URT_BUILD_1048522);
  printf("  game build under test: %s\n",urt_build_name());
  if(image_size&&image_size!=urt_image_bytes()){printf("FAIL SizeOfImage %#x does not match the build under test\n",image_size);return 0;}
  return 1;
}
static uint32_t RV(uint32_t r){
  uint32_t m=urt_rva(r);
  if(m||!r||urt_build()==URT_BUILD_1048522)return m;
  for(unsigned i=0;i<sizeof kTestOnly1090142/sizeof kTestOnly1090142[0];i++)if(kTestOnly1090142[i][0]==r)return kTestOnly1090142[i][1];
  printf("FAIL test address %#x has no build-1090142 mapping\n",r);return 0;
}
#endif
