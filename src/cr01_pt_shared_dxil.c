/* CR01 one-container source-owned DXIL transformation (LC44 8-edit semantic
 * reference factored out of legacy slot replacement). No game executable writes,
 * no shader-slot overwrites, no GPU state-object manipulation. Compile only.
 * Runtime adapter must supply verified DXC DxcCreateInstance from trusted game
 * files and must prove output is accepted by real CreateStateObject. */
#include <stdint.h>
#include <stddef.h>
#define WINAPI __stdcall
#define NULLPTR ((void*)0)
#define TRUE 1
#define FALSE 0
#define DXC_CP_ACP 0
#define DXC_CP_UTF8 65001
#define E_FAIL ((HRESULT)0x80004005L)
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long HRESULT;
typedef int BOOL;
typedef void *LPVOID;
typedef const void *LPCVOID;
typedef unsigned long long SIZE_T;
#define IMP __declspec(dllimport)
IMP void* WINAPI VirtualAlloc(void*,size_t,DWORD,DWORD);
IMP int WINAPI VirtualFree(void*,size_t,DWORD);
static void* rpt_alloc(size_t n){return n?VirtualAlloc(0,n,0x1000|0x2000,0x04):0;}
static void rpt_free(void*p){if(p)(void)VirtualFree(p,0,0x8000);}
static DWORD alen(const char*s){DWORD n=0;while(s&&s[n])n++;return n;}
static uint32_t rd32(const BYTE*s){return (uint32_t)s[0]|((uint32_t)s[1]<<8)|((uint32_t)s[2]<<16)|((uint32_t)s[3]<<24);}
static void wr32(BYTE*s,uint32_t n){for(unsigned i=0;i<4;i++)s[i]=(BYTE)(n>>(8*i));}
typedef struct _RPT_GUID { DWORD Data1; WORD Data2; WORD Data3; BYTE Data4[8]; } RPT_GUID;
typedef struct _IDxcBlob IDxcBlob;
typedef struct _IDxcBlobEncoding IDxcBlobEncoding;
typedef struct _IDxcLibrary IDxcLibrary;
typedef struct _IDxcCompiler IDxcCompiler;
typedef struct _IDxcAssembler IDxcAssembler;
typedef struct _IDxcContainerBuilder IDxcContainerBuilder;
typedef struct _IDxcValidator IDxcValidator;
typedef struct _IDxcOperationResult IDxcOperationResult;

typedef struct _IDxcBlobVtbl {
    HRESULT (WINAPI* QueryInterface)(IDxcBlob*,const RPT_GUID*,LPVOID*);
    DWORD (WINAPI* AddRef)(IDxcBlob*);
    DWORD (WINAPI* Release)(IDxcBlob*);
    LPVOID (WINAPI* GetBufferPointer)(IDxcBlob*);
    SIZE_T (WINAPI* GetBufferSize)(IDxcBlob*);
} IDxcBlobVtbl;
struct _IDxcBlob { IDxcBlobVtbl* lpVtbl; };
struct _IDxcBlobEncoding { IDxcBlobVtbl* lpVtbl; };

typedef struct _IDxcLibraryVtbl {
    HRESULT (WINAPI* QueryInterface)(IDxcLibrary*,const RPT_GUID*,LPVOID*);
    DWORD (WINAPI* AddRef)(IDxcLibrary*);
    DWORD (WINAPI* Release)(IDxcLibrary*);
    LPVOID SetMalloc;
    LPVOID CreateBlobFromBlob;
    LPVOID CreateBlobFromFile;
    HRESULT (WINAPI* CreateBlobWithEncodingFromPinned)(IDxcLibrary*,LPCVOID,DWORD,DWORD,IDxcBlobEncoding**);
    LPVOID CreateBlobWithEncodingOnHeapCopy;
    LPVOID CreateBlobWithEncodingOnMalloc;
    LPVOID CreateIncludeHandler;
    LPVOID CreateStreamFromBlobReadOnly;
    LPVOID GetBlobAsUtf8;
    LPVOID GetBlobAsWide;
} IDxcLibraryVtbl;
struct _IDxcLibrary { IDxcLibraryVtbl* lpVtbl; };

typedef struct _IDxcCompilerVtbl {
    HRESULT (WINAPI* QueryInterface)(IDxcCompiler*,const RPT_GUID*,LPVOID*);
    DWORD (WINAPI* AddRef)(IDxcCompiler*);
    DWORD (WINAPI* Release)(IDxcCompiler*);
    LPVOID Compile;
    LPVOID Preprocess;
    HRESULT (WINAPI* Disassemble)(IDxcCompiler*,IDxcBlob*,IDxcBlobEncoding**);
} IDxcCompilerVtbl;
struct _IDxcCompiler { IDxcCompilerVtbl* lpVtbl; };

typedef struct _IDxcAssemblerVtbl {
    HRESULT (WINAPI* QueryInterface)(IDxcAssembler*,const RPT_GUID*,LPVOID*);
    DWORD (WINAPI* AddRef)(IDxcAssembler*);
    DWORD (WINAPI* Release)(IDxcAssembler*);
    HRESULT (WINAPI* AssembleToContainer)(IDxcAssembler*,IDxcBlob*,IDxcOperationResult**);
} IDxcAssemblerVtbl;
struct _IDxcAssembler { IDxcAssemblerVtbl* lpVtbl; };

typedef struct _IDxcContainerBuilderVtbl {
    HRESULT (WINAPI* QueryInterface)(IDxcContainerBuilder*,const RPT_GUID*,LPVOID*);
    DWORD (WINAPI* AddRef)(IDxcContainerBuilder*);
    DWORD (WINAPI* Release)(IDxcContainerBuilder*);
    HRESULT (WINAPI* Load)(IDxcContainerBuilder*,IDxcBlob*);
    HRESULT (WINAPI* AddPart)(IDxcContainerBuilder*,DWORD,IDxcBlob*);
    HRESULT (WINAPI* RemovePart)(IDxcContainerBuilder*,DWORD);
    HRESULT (WINAPI* SerializeContainer)(IDxcContainerBuilder*,IDxcOperationResult**);
} IDxcContainerBuilderVtbl;
struct _IDxcContainerBuilder { IDxcContainerBuilderVtbl* lpVtbl; };

typedef struct _IDxcValidatorVtbl {
    HRESULT (WINAPI* QueryInterface)(IDxcValidator*,const RPT_GUID*,LPVOID*);
    DWORD (WINAPI* AddRef)(IDxcValidator*);
    DWORD (WINAPI* Release)(IDxcValidator*);
    HRESULT (WINAPI* Validate)(IDxcValidator*,IDxcBlob*,DWORD,IDxcOperationResult**);
} IDxcValidatorVtbl;
struct _IDxcValidator { IDxcValidatorVtbl* lpVtbl; };

typedef struct _IDxcOperationResultVtbl {
    HRESULT (WINAPI* QueryInterface)(IDxcOperationResult*,const RPT_GUID*,LPVOID*);
    DWORD (WINAPI* AddRef)(IDxcOperationResult*);
    DWORD (WINAPI* Release)(IDxcOperationResult*);
    HRESULT (WINAPI* GetStatus)(IDxcOperationResult*,HRESULT*);
    HRESULT (WINAPI* GetResult)(IDxcOperationResult*,IDxcBlob**);
    HRESULT (WINAPI* GetErrorBuffer)(IDxcOperationResult*,IDxcBlobEncoding**);
} IDxcOperationResultVtbl;
struct _IDxcOperationResult { IDxcOperationResultVtbl* lpVtbl; };

typedef HRESULT (WINAPI* PFN_DxcCreateInstance)(const RPT_GUID*,const RPT_GUID*,LPVOID*);

static const RPT_GUID RPT_CLSID_DxcLibrary={0x6245d6af,0x66e0,0x48fd,{0x80,0xb4,0x4d,0x27,0x17,0x96,0x74,0x8c}};
static const RPT_GUID RPT_IID_IDxcLibrary={0xe5204dc7,0xd18c,0x4c3c,{0xbd,0xfb,0x85,0x16,0x73,0x98,0x0f,0xe7}};
static const RPT_GUID RPT_CLSID_DxcCompiler={0x73e22d93,0xe6ce,0x47f3,{0xb5,0xbf,0xf0,0x66,0x4f,0x39,0xc1,0xb0}};
static const RPT_GUID RPT_IID_IDxcCompiler={0x8c210bf3,0x011f,0x4422,{0x8d,0x70,0x6f,0x9a,0xcb,0x8d,0xb6,0x17}};
static const RPT_GUID RPT_CLSID_DxcAssembler={0xd728db68,0xf903,0x4f80,{0x94,0xcd,0xdc,0xcf,0x76,0xec,0x71,0x51}};
static const RPT_GUID RPT_IID_IDxcAssembler={0x091f7a26,0x1c1f,0x4948,{0x90,0x4b,0xe6,0xe3,0xa8,0xa7,0x71,0xd5}};
static const RPT_GUID RPT_CLSID_DxcValidator={0x8ca3e215,0xf728,0x4cf3,{0x8c,0xdd,0x88,0xaf,0x91,0x75,0x87,0xa1}};
static const RPT_GUID RPT_IID_IDxcValidator={0xa6e82bd2,0x1fd7,0x4826,{0x98,0x11,0x28,0x57,0xe7,0x97,0xf4,0x9a}};

static BOOL rpt_eq_at(const char* b,DWORD n,DWORD pos,const char* q){DWORD i=0;if(pos>=n)return FALSE;while(q[i]){if(pos+i>=n||b[pos+i]!=q[i])return FALSE;i++;}return TRUE;}
static DWORD rpt_find(const char* b,DWORD n,const char* q,DWORD from,DWORD to){DWORD i;if(to>n)to=n;for(i=from;i<to;i++)if(rpt_eq_at(b,n,i,q))return i;return 0xffffffffUL;}
static DWORD rpt_line_start(const char* b,DWORD pos){while(pos>0&&b[pos-1]!='\n')pos--;return pos;}
static DWORD rpt_line_end(const char* b,DWORD n,DWORD pos){while(pos<n&&b[pos]!='\n')pos++;if(pos<n)pos++;return pos;}
static BOOL rpt_token(const char* b,DWORD start,DWORD end,DWORD* io,char* out,DWORD cap){DWORD p=*io,n=0;(void)start;while(p<end&&(b[p]==' '||b[p]=='\t'))p++;while(p<end&&b[p]!=' '&&b[p]!='\t'&&b[p]!=','&&b[p]!='\r'&&b[p]!='\n'){if(n+1<cap)out[n++]=b[p];p++;}out[n]=0;*io=p;return n>0;}
static BOOL rpt_append_raw(char* out,DWORD cap,DWORD* used,const char* p,DWORD n){DWORD i;if(!out||!used||!p||*used>=cap||n>=cap-*used)return FALSE;for(i=0;i<n;i++)out[(*used)++]=p[i];out[*used]=0;return TRUE;}
static BOOL rpt_append_str(char* out,DWORD cap,DWORD* used,const char* p){return rpt_append_raw(out,cap,used,p,alen(p));}

/* native: 5.01 source-owned eight-edit validated DXIL transformation.
   Per PT variant: one CameraCB-driven bounce selector and precisely THREE RGB
   outer-bounce accumulation sites. Shader numbering is never hard-coded. The
   three sites MUST feed zero-initialized loop PHIs; otherwise fail closed.
   The two PT functions together must account for eight verified edits.
   This is private evaluation reconstruction, not a validated FPS result. */
static __declspec(noinline) BOOL rpt_patch_one_function_parity(const char* src,DWORD srcn,const char* fnmarker,char* dst,DWORD cap,DWORD* outn){
    typedef struct {DWORD ls,le;char lhs[64],arg[64],acc[64];} RPT_RGB_EDIT;
    RPT_RGB_EDIT edit[3];
    DWORD fs,fe,p,camcount=0,cammark=0xffffffffUL,camls,camle;
    DWORD selmark=0xffffffffUL,selcount=0,ls,le,tp,used=0,after=0,nedits=0,i;
    char cam[64],bounce[64],hair[64],needle[200];
    const char* selneedle=", i32 4, i32 2";
    const char* camneedle="%dx.types.ResourceProperties { i32 13, i32 1632 })";
    const char* addneedle=" = fadd fast float ";
    fs=rpt_find(src,srcn,fnmarker,0,srcn);if(fs==0xffffffffUL)return FALSE;
    fe=rpt_find(src,srcn,"\n}",fs,srcn);if(fe==0xffffffffUL)return FALSE;
    fe=rpt_line_end(src,srcn,fe+1);
    if(rpt_find(src,srcn,"%rpt_",fs,fe)!=0xffffffffUL)return FALSE;
    p=fs;
    while((p=rpt_find(src,srcn,camneedle,p,fe))!=0xffffffffUL){cammark=p;camcount++;p++;}
    if(camcount!=1)return FALSE;
    camls=rpt_line_start(src,cammark);camle=rpt_line_end(src,srcn,cammark);tp=camls;
    if(!rpt_token(src,camls,camle,&tp,cam,sizeof(cam))||cam[0]!='%')return FALSE;
    if(rpt_find(src,srcn,"@CameraCB",fs,camls)==0xffffffffUL)return FALSE;
    p=fs;
    while((p=rpt_find(src,srcn,selneedle,p,fe))!=0xffffffffUL){
        DWORD xls=rpt_line_start(src,p);
        if(rpt_find(src,srcn," = select i1 ",xls,p)!=0xffffffffUL){selmark=p;selcount++;}
        p++;
    }
    if(selcount!=1)return FALSE;
    ls=rpt_line_start(src,selmark);le=rpt_line_end(src,srcn,selmark);
    tp=ls;if(!rpt_token(src,ls,le,&tp,bounce,sizeof(bounce))||bounce[0]!='%')return FALSE;
    p=rpt_find(src,srcn," = select i1 ",ls,le);
    if(p==0xffffffffUL)return FALSE;p+=alen(" = select i1 ");tp=p;
    if(!rpt_token(src,p,le,&tp,hair,sizeof(hair))||hair[0]!='%')return FALSE;

    /* Collect three, and only three, true loop-back radiance summations.
       Pattern: %result = fadd fast float %contribution, %accumulator
       accompanied by a PHI with zero on entry and %result on the loop edge. */
    p=le;
    while((p=rpt_find(src,srcn,addneedle,p,fe))!=0xffffffffUL){
        DWORD fls=rpt_line_start(src,p),fle=rpt_line_end(src,srcn,p),q,phi;
        char lhs[64],arg[64],acc[64];DWORD k=0;
        tp=fls;
        if(!rpt_token(src,fls,fle,&tp,lhs,sizeof(lhs))||lhs[0]!='%'){p=fle;continue;}
        q=p+alen(addneedle);tp=q;
        if(!rpt_token(src,q,fle,&tp,arg,sizeof(arg))||arg[0]!='%'){p=fle;continue;}
        q=tp;while(q<fle&&(src[q]==' '||src[q]==','))q++;tp=q;
        if(!rpt_token(src,q,fle,&tp,acc,sizeof(acc))||acc[0]!='%'){p=fle;continue;}
        /* Require exactly a two-predecessor accumulator PHI, with this result
           fed back, anchored in the same outer loop as the bounce selector. */
        needle[0]=0;
        if(!rpt_append_str(needle,sizeof(needle),&k,acc) ||
           !rpt_append_str(needle,sizeof(needle),&k," = phi float [ 0.000000e+00, "))return FALSE;
        phi=rpt_find(src,srcn,needle,le,fls);
        if(phi!=0xffffffffUL){
            DWORD pl=rpt_line_start(src,phi),pe=rpt_line_end(src,srcn,phi),i2=0;
            char recv[96];recv[0]=0;
            if(!rpt_append_str(recv,sizeof(recv),&i2," ], [ ") ||
               !rpt_append_str(recv,sizeof(recv),&i2,lhs) ||
               !rpt_append_str(recv,sizeof(recv),&i2,", "))return FALSE;
            if(rpt_find(src,srcn,recv,pl,pe)!=0xffffffffUL){
                if(nedits>=3)return FALSE;
                edit[nedits].ls=fls;edit[nedits].le=fle;
                {DWORD z=0;while(lhs[z]){edit[nedits].lhs[z]=lhs[z];z++;}edit[nedits].lhs[z]=0;}
                {DWORD z=0;while(arg[z]){edit[nedits].arg[z]=arg[z];z++;}edit[nedits].arg[z]=0;}
                {DWORD z=0;while(acc[z]){edit[nedits].acc[z]=acc[z];z++;}edit[nedits].acc[z]=0;}
                nedits++;
            }
        }
        p=fle;
    }
    if(nedits!=3 || edit[0].ls<=le || edit[1].ls<edit[0].le || edit[2].ls<edit[1].le)return FALSE;
    /* All RGB adds must form the same contiguous triple immediately before
       the outer bounce-loop counter, never an unrelated inner loop. */
    if(edit[1].ls!=edit[0].le || edit[2].ls!=edit[1].le ||
       rpt_find(src,srcn,"icmp ult i32 ",edit[2].le,edit[2].le+250UL)==0xffffffffUL)return FALSE;

    if(!rpt_append_raw(dst,cap,&used,src,ls))return FALSE;
    if(!rpt_append_str(dst,cap,&used,"  %rpt_raw_cb = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle "))return FALSE;
    if(!rpt_append_str(dst,cap,&used,cam))return FALSE;
    if(!rpt_append_str(dst,cap,&used,", i32 33)\n  %rpt_raw = extractvalue %dx.types.CBufRet.i32 %rpt_raw_cb, 0\n  %rpt_nb = and i32 %rpt_raw, 255\n  %rpt_cp1 = lshr i32 %rpt_raw, 8\n  %rpt_cp = and i32 %rpt_cp1, 255\n  %rpt_cpz = icmp eq i32 %rpt_cp, 0\n  %rpt_cpf = uitofp i32 %rpt_cp to float\n  %rpt_cpd = fmul fast float %rpt_cpf, 0x3F847AE140000000\n  %rpt_comp = select i1 %rpt_cpz, float 1.000000e+00, float %rpt_cpd\n  %rpt_nh_cb = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle "))return FALSE;
    if(!rpt_append_str(dst,cap,&used,cam))return FALSE;
    if(!rpt_append_str(dst,cap,&used,", i32 56)\n  %rpt_nh = extractvalue %dx.types.CBufRet.i32 %rpt_nh_cb, 0\n  "))return FALSE;
    if(!rpt_append_str(dst,cap,&used,bounce)||!rpt_append_str(dst,cap,&used," = select i1 ") ||
       !rpt_append_str(dst,cap,&used,hair)||!rpt_append_str(dst,cap,&used,", i32 %rpt_nh, i32 %rpt_nb\n"))return FALSE;
    after=le;
    for(i=0;i<3;i++){
        if(!rpt_append_raw(dst,cap,&used,src+after,edit[i].ls-after))return FALSE;
        if(!rpt_append_str(dst,cap,&used,"  %rpt_rgb"))return FALSE;
        {char digit=(char)('0'+i);if(!rpt_append_raw(dst,cap,&used,&digit,1))return FALSE;}
        if(!rpt_append_str(dst,cap,&used," = fmul fast float ")||!rpt_append_str(dst,cap,&used,edit[i].arg)||!rpt_append_str(dst,cap,&used,", %rpt_comp\n  "))return FALSE;
        if(!rpt_append_str(dst,cap,&used,edit[i].lhs)||!rpt_append_str(dst,cap,&used," = fadd fast float %rpt_rgb"))return FALSE;
        {char digit=(char)('0'+i);if(!rpt_append_raw(dst,cap,&used,&digit,1))return FALSE;}
        if(!rpt_append_str(dst,cap,&used,", ")||!rpt_append_str(dst,cap,&used,edit[i].acc)||!rpt_append_str(dst,cap,&used,"\n"))return FALSE;
        after=edit[i].le;
    }
    if(!rpt_append_raw(dst,cap,&used,src+after,srcn-after))return FALSE;
    *outn=used;return TRUE;
}


/* R15: strict SSA-gated indirect-only contributions in BOTH native 5.01
 * exports. Exact SSA anchors belong to the verified native shared shader:
 * they are rejected if either function's depth PHI, loop add, or terminal add
 * changes. The upstream shared slot is cryptographically version-gated.
 * R14 final-output diagnostic is deliberately absent. */
static BOOL rpt_replace_in_export(const char*src,DWORD srcn,const char*fn,
   const char*needle,const char*replacement,char*dst,DWORD cap,DWORD*outn){
 DWORD fs=rpt_find(src,srcn,fn,0,srcn),fe,hit,again,used=0;
 if(fs==0xffffffffUL)return FALSE;
 fe=rpt_find(src,srcn,"\n}",fs,srcn);if(fe==0xffffffffUL)return FALSE;
 hit=rpt_find(src,srcn,needle,fs,fe);if(hit==0xffffffffUL)return FALSE;
 again=rpt_find(src,srcn,needle,hit+1,fe);if(again!=0xffffffffUL)return FALSE;
 if(!rpt_append_raw(dst,cap,&used,src,hit)||
    !rpt_append_str(dst,cap,&used,replacement)||
    !rpt_append_raw(dst,cap,&used,src+hit+alen(needle),srcn-hit-alen(needle)))return FALSE;
 *outn=used;return TRUE;
}
typedef struct {const char*old;const char*newtext;} R15Replacement;
static BOOL rpt_r15_one(const char*src,DWORD srcn,const char*fn,
  const R15Replacement*repl,DWORD count,char*dst,DWORD cap,DWORD*outn){
 char *a=(char*)rpt_alloc(cap),*b=(char*)rpt_alloc(cap);
 const char*cur=src;DWORD n=srcn,m=0,i;BOOL ok=FALSE;
 if(!a||!b)goto done;
 for(i=0;i<count;i++){
  char*target=(i+1==count)?dst:((i&1)?b:a);
  if(!rpt_replace_in_export(cur,n,fn,repl[i].old,repl[i].newtext,target,cap,&m))goto done;
  cur=target;n=m;
 }
 *outn=n;ok=TRUE;
 done:
 if(a)rpt_free(a);if(b)rpt_free(b);return ok;
}
#define R15_LOOP_GUARD(depth) "  %rpt_p2_is_secondary = icmp ne i32 " depth ", 0\n" \
 "  %rpt_p2_low = icmp uge i32 %rpt_cp, 101\n" \
 "  %rpt_p2_high = icmp ule i32 %rpt_cp, 150\n" \
 "  %rpt_p2_valid = and i1 %rpt_p2_low, %rpt_p2_high\n" \
 "  %rpt_p2_apply = and i1 %rpt_p2_is_secondary, %rpt_p2_valid\n"
#define R15_LOOP(k,raw,acc,output) \
 {"  " output " = fadd fast float %rpt_rgb" k ", " acc "\n", \
  "  %rpt_p2_loop_choose" k " = select i1 %rpt_p2_apply, float %rpt_rgb" k ", float " raw "\n" \
  "  " output " = fadd fast float %rpt_p2_loop_choose" k ", " acc "\n"}
#define R15_TERM(group,k,raw,acc,output) \
 {"  " output " = fadd fast float " raw ", " acc "\n", \
 "  %rpt_p2_t" group "_scaled" k " = fmul fast float " raw ", %rpt_comp\n" \
 "  %rpt_p2_t" group "_choose" k " = select i1 %rpt_p2_apply, float %rpt_p2_t" group "_scaled" k ", float " raw "\n" \
 "  " output " = fadd fast float %rpt_p2_t" group "_choose" k ", " acc "\n"}
/* Private R15 terminal-secondary correction candidate: the non-cache terminal
 * (R15_TERM group 1, historically labelled T2) is reached ONLY
 * after another GbufferPayload TraceRay inside the already-entered path loop.
 * Thus loop index 0 may already carry first-bounce radiance. The strict user
 * byte gate still enforces 101..150 and exact neutral at 100; other groups
 * remain governed by depth>0. Runtime/GPU behavior still requires validation. */
#define R15_POSTTRACE_TERM(group,k,raw,acc,output) \
 {"  " output " = fadd fast float " raw ", " acc "\n", \
 "  %rpt_p2_t" group "_scaled" k " = fmul fast float " raw ", %rpt_comp\n" \
 "  %rpt_p2_t" group "_choose" k " = select i1 %rpt_p2_valid, float %rpt_p2_t" group "_scaled" k ", float " raw "\n" \
 "  " output " = fadd fast float %rpt_p2_t" group "_choose" k ", " acc "\n"}
static BOOL rpt_patch_ir_c17_base(const char*src,DWORD srcn,char*out,DWORD cap,DWORD*outn){
 static const R15Replacement final[]={
 {"  %963 = phi i32 [ 0, %900 ], [ %15429, %15401 ]\n",
  "  %963 = phi i32 [ 0, %900 ], [ %15429, %15401 ]\n" R15_LOOP_GUARD("%963")},
 R15_LOOP("0","%15423","%934","%15426"),
 R15_LOOP("1","%15424","%935","%15427"),
 R15_LOOP("2","%15425","%936","%15428"),
 R15_TERM("0","0","%5828","%934","%5833"),
 R15_TERM("0","1","%5830","%935","%5834"),
 R15_TERM("0","2","%5832","%936","%5835"),
 R15_POSTTRACE_TERM("1","0","%7111","%934","%7114"),
 R15_POSTTRACE_TERM("1","1","%7112","%935","%7115"),
 R15_POSTTRACE_TERM("1","2","%7113","%936","%7116")};
 static const R15Replacement ser[]={
 {"  %965 = phi i32 [ 0, %902 ], [ %15496, %15468 ]\n",
  "  %965 = phi i32 [ 0, %902 ], [ %15496, %15468 ]\n" R15_LOOP_GUARD("%965")},
 R15_LOOP("0","%15490","%936","%15493"),
 R15_LOOP("1","%15491","%937","%15494"),
 R15_LOOP("2","%15492","%938","%15495"),
 R15_TERM("0","0","%5830","%936","%5835"),
 R15_TERM("0","1","%5832","%937","%5836"),
 R15_TERM("0","2","%5834","%938","%5837"),
 R15_POSTTRACE_TERM("1","0","%7160","%936","%7163"),
 R15_POSTTRACE_TERM("1","1","%7161","%937","%7164"),
 R15_POSTTRACE_TERM("1","2","%7162","%938","%7165")};
 const char*f1="define void @\"\\01?PTRGSFinal@@YAXXZ\"()";
 const char*f2="define void @\"\\01?PTRGSWithSERFinal@@YAXXZ\"()";
 char*t1=(char*)rpt_alloc(cap),*t2=(char*)rpt_alloc(cap);
 DWORD n1=0,n2=0,n3=0,n4=0;BOOL ok=FALSE;
 if(!t1||!t2)goto done;
 if(!rpt_patch_one_function_parity(src,srcn,f1,t1,cap,&n1))goto done;
 if(!rpt_r15_one(t1,n1,f1,final,sizeof(final)/sizeof(final[0]),t2,cap,&n2))goto done;
 if(!rpt_patch_one_function_parity(t2,n2,f2,t1,cap,&n3))goto done;
 if(!rpt_r15_one(t1,n3,f2,ser,sizeof(ser)/sizeof(ser[0]),out,cap,&n4))goto done;
 if(rpt_find(out,n4,"%rpt_out",0,n4)!=0xffffffffUL)goto done;
 *outn=n4;ok=TRUE;
 done:
 if(t1)rpt_free(t1);if(t2)rpt_free(t2);return ok;
}


/* EXPERIMENTAL SHADER-FIRST RTAO IR proof. NOT RELEASE ACCEPTED.
 * Adds a bounded TLAS GbufferPayload trace inside BOTH native PT raygen
 * exports, guarded by existing synchronous CameraCB word != native 2.
 * Intended to gate only first-secondary loop radiance. No new bindings/SBT.
 * Exact DXC assembly, state-object creation and GPU effect UNVERIFIED. */
static const R15Replacement rpt_ao_final[]={
{"  %30 = alloca %struct.GbufferPayload, align 8\n"
 "",
"  %30 = alloca %struct.GbufferPayload, align 8\n"
 "  %rpt_ao_payload = alloca %struct.GbufferPayload, align 8\n"
 ""},
{"  %907 = select i1 %137, i32 %rpt_nh, i32 %rpt_nb\n"
 "  br label %908\n" "",
 "  %907 = select i1 %137, i32 %rpt_nh, i32 %rpt_nb\n"
 "  %rpt_ao_cb = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle %34, i32 33)\n"
 "  %rpt_ao_raw = extractvalue %dx.types.CBufRet.i32 %rpt_ao_cb, 0\n"
 "  %rpt_ao_flagword = lshr i32 %rpt_ao_raw, 24\n"
 "  %rpt_ao_flag = and i32 %rpt_ao_flagword, 1\n"
 "  %rpt_ao_enable = icmp eq i32 %rpt_ao_flag, 1\n"
 "  %rpt_ao_strraw = lshr i32 %rpt_ao_raw, 21\n"
 "  %rpt_ao_stridx = and i32 %rpt_ao_strraw, 7\n"
 "  %rpt_ao_zero = icmp eq i32 %rpt_ao_stridx, 1\n"
 "  %rpt_ao_notzero = xor i1 %rpt_ao_zero, true\n"
 "  %rpt_ao_on = and i1 %rpt_ao_enable, %rpt_ao_notzero\n"
 "  br i1 %rpt_ao_on, label %rpt_ao_compute, label %rpt_ao_skip\n"
 "\n"
 "rpt_ao_compute: ; preds = %900\n"
 "  %rpt_ao_raybits = lshr i32 %rpt_ao_raw, 16\n"
 "  %rpt_ao_rayidx = and i32 %rpt_ao_raybits, 3\n"
 "  %rpt_ao_count = shl i32 1, %rpt_ao_rayidx\n"
 "  %rpt_ao_radbits = lshr i32 %rpt_ao_raw, 18\n"
 "  %rpt_ao_radidx = and i32 %rpt_ao_radbits, 7\n"
 "  %rpt_ao_rad_is1 = icmp eq i32 %rpt_ao_radidx, 1\n"
 "  %rpt_ao_rad_v1 = select i1 %rpt_ao_rad_is1, float 2.500000e-01, float 1.250000e+00\n"
 "  %rpt_ao_rad_is2 = icmp eq i32 %rpt_ao_radidx, 2\n"
 "  %rpt_ao_rad_v2 = select i1 %rpt_ao_rad_is2, float 5.000000e-01, float %rpt_ao_rad_v1\n"
 "  %rpt_ao_rad_is3 = icmp eq i32 %rpt_ao_radidx, 3\n"
 "  %rpt_ao_rad_v3 = select i1 %rpt_ao_rad_is3, float 7.500000e-01, float %rpt_ao_rad_v2\n"
 "  %rpt_ao_rad_is4 = icmp eq i32 %rpt_ao_radidx, 4\n"
 "  %rpt_ao_rad_v4 = select i1 %rpt_ao_rad_is4, float 2.000000e+00, float %rpt_ao_rad_v3\n"
 "  %rpt_ao_rad_is5 = icmp eq i32 %rpt_ao_radidx, 5\n"
 "  %rpt_ao_rad_v5 = select i1 %rpt_ao_rad_is5, float 3.000000e+00, float %rpt_ao_rad_v4\n"
 "  %rpt_ao_rad_is6 = icmp eq i32 %rpt_ao_radidx, 6\n"
 "  %rpt_ao_rad_v6 = select i1 %rpt_ao_rad_is6, float 4.000000e+00, float %rpt_ao_rad_v5\n"
 "  %rpt_ao_radius = fadd float %rpt_ao_rad_v6, 0.000000e+00\n"
 "  %rpt_ao_str_is1 = icmp eq i32 %rpt_ao_stridx, 1\n"
 "  %rpt_ao_str_v1 = select i1 %rpt_ao_str_is1, float 0.000000e+00, float 0x3FE4CCCCC0000000\n"
 "  %rpt_ao_str_is2 = icmp eq i32 %rpt_ao_stridx, 2\n"
 "  %rpt_ao_str_v2 = select i1 %rpt_ao_str_is2, float 2.500000e-01, float %rpt_ao_str_v1\n"
 "  %rpt_ao_str_is3 = icmp eq i32 %rpt_ao_stridx, 3\n"
 "  %rpt_ao_str_v3 = select i1 %rpt_ao_str_is3, float 0x3FD99999A0000000, float %rpt_ao_str_v2\n"
 "  %rpt_ao_str_is4 = icmp eq i32 %rpt_ao_stridx, 4\n"
 "  %rpt_ao_str_v4 = select i1 %rpt_ao_str_is4, float 0x3FE99999A0000000, float %rpt_ao_str_v3\n"
 "  %rpt_ao_str_is5 = icmp eq i32 %rpt_ao_stridx, 5\n"
 "  %rpt_ao_str_v5 = select i1 %rpt_ao_str_is5, float 1.000000e+00, float %rpt_ao_str_v4\n"
 "  %rpt_ao_strength = fadd float %rpt_ao_str_v5, 0.000000e+00\n"
 "  %rpt_ao_oxoff = fmul float %519, 0x3F9EB851E0000000\n"
 "  %rpt_ao_oyoff = fmul float %520, 0x3F9EB851E0000000\n"
 "  %rpt_ao_ozoff = fmul float %521, 0x3F9EB851E0000000\n"
 "  %rpt_ao_ox = fadd float %399, %rpt_ao_oxoff\n"
 "  %rpt_ao_oy = fadd float %400, %rpt_ao_oyoff\n"
 "  %rpt_ao_oz = fadd float %401, %rpt_ao_ozoff\n"
 "  %rpt_ao_tlas = call %dx.types.Handle @dx.op.createHandleForLib.dx.types.Handle(i32 160, %dx.types.Handle %19)\n"
 "  %rpt_ao_tlas_ann = call %dx.types.Handle @dx.op.annotateHandle(i32 216, %dx.types.Handle %rpt_ao_tlas, %dx.types.ResourceProperties { i32 16, i32 0 })\n"
 "  %rpt_ao_vec = getelementptr inbounds %struct.GbufferPayload, %struct.GbufferPayload* %rpt_ao_payload, i32 0, i32 0\n"
 "  %rpt_ao_flags = getelementptr inbounds %struct.GbufferPayload, %struct.GbufferPayload* %rpt_ao_payload, i32 0, i32 1\n"
 "  br label %rpt_ao_loop\n"
 "\n"
 "rpt_ao_loop: ; preds = %rpt_ao_compute, %rpt_ao_loop\n"
 "  %rpt_ao_i = phi i32 [ 0, %rpt_ao_compute ], [ %rpt_ao_inext, %rpt_ao_loop ]\n"
 "  %rpt_ao_hits = phi float [ 0.000000e+00, %rpt_ao_compute ], [ %rpt_ao_hits_next, %rpt_ao_loop ]\n"
 "  %rpt_ao_hx = mul i32 %35, 73856093\n"
 "  %rpt_ao_hy = mul i32 %36, 19349663\n"
 "  %rpt_ao_seed = xor i32 %rpt_ao_hx, %rpt_ao_hy\n"
 "  %rpt_ao_iterm = mul i32 %rpt_ao_i, -1640531527\n"
 "  %rpt_ao_hash = xor i32 %rpt_ao_seed, %rpt_ao_iterm\n"
 "  %rpt_ao_ha = and i32 %rpt_ao_hash, 65535\n"
 "  %rpt_ao_hb0 = lshr i32 %rpt_ao_hash, 16\n"
 "  %rpt_ao_hb = and i32 %rpt_ao_hb0, 65535\n"
 "  %rpt_ao_fa = uitofp i32 %rpt_ao_ha to float\n"
 "  %rpt_ao_fb = uitofp i32 %rpt_ao_hb to float\n"
 "  %rpt_ao_a0 = fmul float %rpt_ao_fa, 0x3F00008000000000\n"
 "  %rpt_ao_b0 = fmul float %rpt_ao_fb, 0x3F00008000000000\n"
 "  %rpt_ao_a = fsub float %rpt_ao_a0, 1.000000e+00\n"
 "  %rpt_ao_b = fsub float %rpt_ao_b0, 1.000000e+00\n"
 "  %rpt_ao_xj = fmul float %rpt_ao_a, 0x3FE3333340000000\n"
 "  %rpt_ao_yj = fmul float %rpt_ao_b, 0x3FE3333340000000\n"
 "  %rpt_ao_d0 = fadd float %519, %rpt_ao_xj\n"
 "  %rpt_ao_d1 = fadd float %520, %rpt_ao_yj\n"
 "  %rpt_ao_d2 = fadd float %521, 0x3FD0000000000000\n"
 "  %rpt_ao_len2 = call float @dx.op.dot3.f32(i32 55, float %rpt_ao_d0, float %rpt_ao_d1, float %rpt_ao_d2, float %rpt_ao_d0, float %rpt_ao_d1, float %rpt_ao_d2)\n"
 "  %rpt_ao_invlen = call float @dx.op.unary.f32(i32 25, float %rpt_ao_len2)\n"
 "  %rpt_ao_dx = fmul float %rpt_ao_d0, %rpt_ao_invlen\n"
 "  %rpt_ao_dy = fmul float %rpt_ao_d1, %rpt_ao_invlen\n"
 "  %rpt_ao_dz = fmul float %rpt_ao_d2, %rpt_ao_invlen\n"
 "  store <4 x i32> zeroinitializer, <4 x i32>* %rpt_ao_vec, align 8\n"
 "  store i32 0, i32* %rpt_ao_flags, align 8\n"
 "  call void @dx.op.traceRay.struct.GbufferPayload(i32 157, %dx.types.Handle %rpt_ao_tlas_ann, i32 528, i32 255, i32 0, i32 4, i32 0, float %rpt_ao_ox, float %rpt_ao_oy, float %rpt_ao_oz, float 0x3F847AE140000000, float %rpt_ao_dx, float %rpt_ao_dy, float %rpt_ao_dz, float %rpt_ao_radius, %struct.GbufferPayload* nonnull %rpt_ao_payload)\n"
 "  %rpt_ao_ret = load <4 x i32>, <4 x i32>* %rpt_ao_vec, align 8\n"
 "  %rpt_ao_tbits = extractelement <4 x i32> %rpt_ao_ret, i32 3\n"
 "  %rpt_ao_t = bitcast i32 %rpt_ao_tbits to float\n"
 "  %rpt_ao_hit = fcmp ogt float %rpt_ao_t, 0.000000e+00\n"
 "  %rpt_ao_hitf = select i1 %rpt_ao_hit, float 1.000000e+00, float 0.000000e+00\n"
 "  %rpt_ao_hits_next = fadd float %rpt_ao_hits, %rpt_ao_hitf\n"
 "  %rpt_ao_inext = add nuw i32 %rpt_ao_i, 1\n"
 "  %rpt_ao_more = icmp ult i32 %rpt_ao_inext, %rpt_ao_count\n"
 "  br i1 %rpt_ao_more, label %rpt_ao_loop, label %rpt_ao_done\n"
 "\n"
 "rpt_ao_done: ; preds = %rpt_ao_loop\n"
 "  %rpt_ao_countf = uitofp i32 %rpt_ao_count to float\n"
 "  %rpt_ao_fraction = fdiv float %rpt_ao_hits_next, %rpt_ao_countf\n"
 "  %rpt_ao_amount = fmul float %rpt_ao_fraction, %rpt_ao_strength\n"
 "  %rpt_ao_vis_computed = fsub float 1.000000e+00, %rpt_ao_amount\n"
 "  br label %rpt_ao_join\n"
 "\n"
 "rpt_ao_skip: ; preds = %900\n"
 "  br label %rpt_ao_join\n"
 "\n"
 "rpt_ao_join: ; preds = %rpt_ao_done, %rpt_ao_skip\n"
 "  %rpt_ao_vis = phi float [ %rpt_ao_vis_computed, %rpt_ao_done ], [ 1.000000e+00, %rpt_ao_skip ]\n"
 "  br label %908\n" ""},
{"  %rpt_p2_apply = and i1 %rpt_p2_is_secondary, %rpt_p2_valid\n"
 "",
"  %rpt_p2_apply = and i1 %rpt_p2_is_secondary, %rpt_p2_valid\n"
 "  %rpt_ao_secondary = icmp eq i32 %963, 1\n"
 "  %rpt_ao_indirect = select i1 %rpt_ao_secondary, float %rpt_ao_vis, float 1.000000e+00\n"
 ""},
{"  %15426 = fadd fast float %rpt_p2_loop_choose0, %934\n"
 "",
"  %rpt_ao_color0 = fmul fast float %rpt_p2_loop_choose0, %rpt_ao_indirect\n"
 "  %15426 = fadd fast float %rpt_ao_color0, %934\n"
 ""},
{"  %15427 = fadd fast float %rpt_p2_loop_choose1, %935\n"
 "",
"  %rpt_ao_color1 = fmul fast float %rpt_p2_loop_choose1, %rpt_ao_indirect\n"
 "  %15427 = fadd fast float %rpt_ao_color1, %935\n"
 ""},
{"  %15428 = fadd fast float %rpt_p2_loop_choose2, %936\n"
 "",
"  %rpt_ao_color2 = fmul fast float %rpt_p2_loop_choose2, %rpt_ao_indirect\n"
 "  %15428 = fadd fast float %rpt_ao_color2, %936\n"
 ""},
};
static const R15Replacement rpt_ao_ser[]={
{"  %32 = alloca %struct.GbufferPayload, align 8\n"
 "",
"  %32 = alloca %struct.GbufferPayload, align 8\n"
 "  %rpt_ao_payload = alloca %struct.GbufferPayload, align 8\n"
 ""},
{"  %909 = select i1 %139, i32 %rpt_nh, i32 %rpt_nb\n"
 "  br label %910\n" "",
 "  %909 = select i1 %139, i32 %rpt_nh, i32 %rpt_nb\n"
 "  %rpt_ao_cb = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle %36, i32 33)\n"
 "  %rpt_ao_raw = extractvalue %dx.types.CBufRet.i32 %rpt_ao_cb, 0\n"
 "  %rpt_ao_flagword = lshr i32 %rpt_ao_raw, 24\n"
 "  %rpt_ao_flag = and i32 %rpt_ao_flagword, 1\n"
 "  %rpt_ao_enable = icmp eq i32 %rpt_ao_flag, 1\n"
 "  %rpt_ao_strraw = lshr i32 %rpt_ao_raw, 21\n"
 "  %rpt_ao_stridx = and i32 %rpt_ao_strraw, 7\n"
 "  %rpt_ao_zero = icmp eq i32 %rpt_ao_stridx, 1\n"
 "  %rpt_ao_notzero = xor i1 %rpt_ao_zero, true\n"
 "  %rpt_ao_on = and i1 %rpt_ao_enable, %rpt_ao_notzero\n"
 "  br i1 %rpt_ao_on, label %rpt_ao_compute, label %rpt_ao_skip\n"
 "\n"
 "rpt_ao_compute: ; preds = %902\n"
 "  %rpt_ao_raybits = lshr i32 %rpt_ao_raw, 16\n"
 "  %rpt_ao_rayidx = and i32 %rpt_ao_raybits, 3\n"
 "  %rpt_ao_count = shl i32 1, %rpt_ao_rayidx\n"
 "  %rpt_ao_radbits = lshr i32 %rpt_ao_raw, 18\n"
 "  %rpt_ao_radidx = and i32 %rpt_ao_radbits, 7\n"
 "  %rpt_ao_rad_is1 = icmp eq i32 %rpt_ao_radidx, 1\n"
 "  %rpt_ao_rad_v1 = select i1 %rpt_ao_rad_is1, float 2.500000e-01, float 1.250000e+00\n"
 "  %rpt_ao_rad_is2 = icmp eq i32 %rpt_ao_radidx, 2\n"
 "  %rpt_ao_rad_v2 = select i1 %rpt_ao_rad_is2, float 5.000000e-01, float %rpt_ao_rad_v1\n"
 "  %rpt_ao_rad_is3 = icmp eq i32 %rpt_ao_radidx, 3\n"
 "  %rpt_ao_rad_v3 = select i1 %rpt_ao_rad_is3, float 7.500000e-01, float %rpt_ao_rad_v2\n"
 "  %rpt_ao_rad_is4 = icmp eq i32 %rpt_ao_radidx, 4\n"
 "  %rpt_ao_rad_v4 = select i1 %rpt_ao_rad_is4, float 2.000000e+00, float %rpt_ao_rad_v3\n"
 "  %rpt_ao_rad_is5 = icmp eq i32 %rpt_ao_radidx, 5\n"
 "  %rpt_ao_rad_v5 = select i1 %rpt_ao_rad_is5, float 3.000000e+00, float %rpt_ao_rad_v4\n"
 "  %rpt_ao_rad_is6 = icmp eq i32 %rpt_ao_radidx, 6\n"
 "  %rpt_ao_rad_v6 = select i1 %rpt_ao_rad_is6, float 4.000000e+00, float %rpt_ao_rad_v5\n"
 "  %rpt_ao_radius = fadd float %rpt_ao_rad_v6, 0.000000e+00\n"
 "  %rpt_ao_str_is1 = icmp eq i32 %rpt_ao_stridx, 1\n"
 "  %rpt_ao_str_v1 = select i1 %rpt_ao_str_is1, float 0.000000e+00, float 0x3FE4CCCCC0000000\n"
 "  %rpt_ao_str_is2 = icmp eq i32 %rpt_ao_stridx, 2\n"
 "  %rpt_ao_str_v2 = select i1 %rpt_ao_str_is2, float 2.500000e-01, float %rpt_ao_str_v1\n"
 "  %rpt_ao_str_is3 = icmp eq i32 %rpt_ao_stridx, 3\n"
 "  %rpt_ao_str_v3 = select i1 %rpt_ao_str_is3, float 0x3FD99999A0000000, float %rpt_ao_str_v2\n"
 "  %rpt_ao_str_is4 = icmp eq i32 %rpt_ao_stridx, 4\n"
 "  %rpt_ao_str_v4 = select i1 %rpt_ao_str_is4, float 0x3FE99999A0000000, float %rpt_ao_str_v3\n"
 "  %rpt_ao_str_is5 = icmp eq i32 %rpt_ao_stridx, 5\n"
 "  %rpt_ao_str_v5 = select i1 %rpt_ao_str_is5, float 1.000000e+00, float %rpt_ao_str_v4\n"
 "  %rpt_ao_strength = fadd float %rpt_ao_str_v5, 0.000000e+00\n"
 "  %rpt_ao_oxoff = fmul float %521, 0x3F9EB851E0000000\n"
 "  %rpt_ao_oyoff = fmul float %522, 0x3F9EB851E0000000\n"
 "  %rpt_ao_ozoff = fmul float %523, 0x3F9EB851E0000000\n"
 "  %rpt_ao_ox = fadd float %401, %rpt_ao_oxoff\n"
 "  %rpt_ao_oy = fadd float %402, %rpt_ao_oyoff\n"
 "  %rpt_ao_oz = fadd float %403, %rpt_ao_ozoff\n"
 "  %rpt_ao_tlas = call %dx.types.Handle @dx.op.createHandleForLib.dx.types.Handle(i32 160, %dx.types.Handle %19)\n"
 "  %rpt_ao_tlas_ann = call %dx.types.Handle @dx.op.annotateHandle(i32 216, %dx.types.Handle %rpt_ao_tlas, %dx.types.ResourceProperties { i32 16, i32 0 })\n"
 "  %rpt_ao_vec = getelementptr inbounds %struct.GbufferPayload, %struct.GbufferPayload* %rpt_ao_payload, i32 0, i32 0\n"
 "  %rpt_ao_flags = getelementptr inbounds %struct.GbufferPayload, %struct.GbufferPayload* %rpt_ao_payload, i32 0, i32 1\n"
 "  br label %rpt_ao_loop\n"
 "\n"
 "rpt_ao_loop: ; preds = %rpt_ao_compute, %rpt_ao_loop\n"
 "  %rpt_ao_i = phi i32 [ 0, %rpt_ao_compute ], [ %rpt_ao_inext, %rpt_ao_loop ]\n"
 "  %rpt_ao_hits = phi float [ 0.000000e+00, %rpt_ao_compute ], [ %rpt_ao_hits_next, %rpt_ao_loop ]\n"
 "  %rpt_ao_hx = mul i32 %37, 73856093\n"
 "  %rpt_ao_hy = mul i32 %38, 19349663\n"
 "  %rpt_ao_seed = xor i32 %rpt_ao_hx, %rpt_ao_hy\n"
 "  %rpt_ao_iterm = mul i32 %rpt_ao_i, -1640531527\n"
 "  %rpt_ao_hash = xor i32 %rpt_ao_seed, %rpt_ao_iterm\n"
 "  %rpt_ao_ha = and i32 %rpt_ao_hash, 65535\n"
 "  %rpt_ao_hb0 = lshr i32 %rpt_ao_hash, 16\n"
 "  %rpt_ao_hb = and i32 %rpt_ao_hb0, 65535\n"
 "  %rpt_ao_fa = uitofp i32 %rpt_ao_ha to float\n"
 "  %rpt_ao_fb = uitofp i32 %rpt_ao_hb to float\n"
 "  %rpt_ao_a0 = fmul float %rpt_ao_fa, 0x3F00008000000000\n"
 "  %rpt_ao_b0 = fmul float %rpt_ao_fb, 0x3F00008000000000\n"
 "  %rpt_ao_a = fsub float %rpt_ao_a0, 1.000000e+00\n"
 "  %rpt_ao_b = fsub float %rpt_ao_b0, 1.000000e+00\n"
 "  %rpt_ao_xj = fmul float %rpt_ao_a, 0x3FE3333340000000\n"
 "  %rpt_ao_yj = fmul float %rpt_ao_b, 0x3FE3333340000000\n"
 "  %rpt_ao_d0 = fadd float %521, %rpt_ao_xj\n"
 "  %rpt_ao_d1 = fadd float %522, %rpt_ao_yj\n"
 "  %rpt_ao_d2 = fadd float %523, 0x3FD0000000000000\n"
 "  %rpt_ao_len2 = call float @dx.op.dot3.f32(i32 55, float %rpt_ao_d0, float %rpt_ao_d1, float %rpt_ao_d2, float %rpt_ao_d0, float %rpt_ao_d1, float %rpt_ao_d2)\n"
 "  %rpt_ao_invlen = call float @dx.op.unary.f32(i32 25, float %rpt_ao_len2)\n"
 "  %rpt_ao_dx = fmul float %rpt_ao_d0, %rpt_ao_invlen\n"
 "  %rpt_ao_dy = fmul float %rpt_ao_d1, %rpt_ao_invlen\n"
 "  %rpt_ao_dz = fmul float %rpt_ao_d2, %rpt_ao_invlen\n"
 "  store <4 x i32> zeroinitializer, <4 x i32>* %rpt_ao_vec, align 8\n"
 "  store i32 0, i32* %rpt_ao_flags, align 8\n"
 "  call void @dx.op.traceRay.struct.GbufferPayload(i32 157, %dx.types.Handle %rpt_ao_tlas_ann, i32 528, i32 255, i32 0, i32 4, i32 0, float %rpt_ao_ox, float %rpt_ao_oy, float %rpt_ao_oz, float 0x3F847AE140000000, float %rpt_ao_dx, float %rpt_ao_dy, float %rpt_ao_dz, float %rpt_ao_radius, %struct.GbufferPayload* nonnull %rpt_ao_payload)\n"
 "  %rpt_ao_ret = load <4 x i32>, <4 x i32>* %rpt_ao_vec, align 8\n"
 "  %rpt_ao_tbits = extractelement <4 x i32> %rpt_ao_ret, i32 3\n"
 "  %rpt_ao_t = bitcast i32 %rpt_ao_tbits to float\n"
 "  %rpt_ao_hit = fcmp ogt float %rpt_ao_t, 0.000000e+00\n"
 "  %rpt_ao_hitf = select i1 %rpt_ao_hit, float 1.000000e+00, float 0.000000e+00\n"
 "  %rpt_ao_hits_next = fadd float %rpt_ao_hits, %rpt_ao_hitf\n"
 "  %rpt_ao_inext = add nuw i32 %rpt_ao_i, 1\n"
 "  %rpt_ao_more = icmp ult i32 %rpt_ao_inext, %rpt_ao_count\n"
 "  br i1 %rpt_ao_more, label %rpt_ao_loop, label %rpt_ao_done\n"
 "\n"
 "rpt_ao_done: ; preds = %rpt_ao_loop\n"
 "  %rpt_ao_countf = uitofp i32 %rpt_ao_count to float\n"
 "  %rpt_ao_fraction = fdiv float %rpt_ao_hits_next, %rpt_ao_countf\n"
 "  %rpt_ao_amount = fmul float %rpt_ao_fraction, %rpt_ao_strength\n"
 "  %rpt_ao_vis_computed = fsub float 1.000000e+00, %rpt_ao_amount\n"
 "  br label %rpt_ao_join\n"
 "\n"
 "rpt_ao_skip: ; preds = %902\n"
 "  br label %rpt_ao_join\n"
 "\n"
 "rpt_ao_join: ; preds = %rpt_ao_done, %rpt_ao_skip\n"
 "  %rpt_ao_vis = phi float [ %rpt_ao_vis_computed, %rpt_ao_done ], [ 1.000000e+00, %rpt_ao_skip ]\n"
 "  br label %910\n" ""},
{"  %rpt_p2_apply = and i1 %rpt_p2_is_secondary, %rpt_p2_valid\n"
 "",
"  %rpt_p2_apply = and i1 %rpt_p2_is_secondary, %rpt_p2_valid\n"
 "  %rpt_ao_secondary = icmp eq i32 %965, 1\n"
 "  %rpt_ao_indirect = select i1 %rpt_ao_secondary, float %rpt_ao_vis, float 1.000000e+00\n"
 ""},
{"  %15493 = fadd fast float %rpt_p2_loop_choose0, %936\n"
 "",
"  %rpt_ao_color0 = fmul fast float %rpt_p2_loop_choose0, %rpt_ao_indirect\n"
 "  %15493 = fadd fast float %rpt_ao_color0, %936\n"
 ""},
{"  %15494 = fadd fast float %rpt_p2_loop_choose1, %937\n"
 "",
"  %rpt_ao_color1 = fmul fast float %rpt_p2_loop_choose1, %rpt_ao_indirect\n"
 "  %15494 = fadd fast float %rpt_ao_color1, %937\n"
 ""},
{"  %15495 = fadd fast float %rpt_p2_loop_choose2, %938\n"
 "",
"  %rpt_ao_color2 = fmul fast float %rpt_p2_loop_choose2, %rpt_ao_indirect\n"
 "  %15495 = fadd fast float %rpt_ao_color2, %938\n"
 ""},
};

/* Retarget only loop-header PHI predecessor from original 900/902 to the
 * AO join block, without altering any other branches/PHIs. */
static BOOL rpt_ao_retarget(const char*src,DWORD n,const char*fn,
  const char*header,const char*oldpred,char*out,DWORD cap,DWORD*outn){
 DWORD fs=rpt_find(src,n,fn,0,n),fe,bs,end,p,used=0,hits=0;
 if(fs==0xffffffffUL)return FALSE;
 fe=rpt_find(src,n,"\n}",fs,n);if(fe==0xffffffffUL)return FALSE;
 bs=rpt_find(src,n,header,fs,fe);if(bs==0xffffffffUL)return FALSE;
 end=rpt_find(src,n," = phi ",bs+alen(header),fe);
 if(end==0xffffffffUL)return FALSE;
 /* Find the first non-PHI instruction after the block label and PHI lines. */
 end=rpt_line_end(src,n,bs+alen(header));
 while(end<fe){
  DWORD next=rpt_line_end(src,n,end);
  if(rpt_find(src,n," = phi ",end,next)==0xffffffffUL)break;
  end=next;
 }
 if(end<=bs||end>fe)return FALSE;
 if(!rpt_append_raw(out,cap,&used,src,bs))return FALSE;
 p=bs;
 while(p<end){
  if(rpt_eq_at(src,n,p,oldpred)){
   if(!rpt_append_str(out,cap,&used,", %rpt_ao_join ]"))return FALSE;
   p+=alen(oldpred);hits++;
  }else{if(!rpt_append_raw(out,cap,&used,src+p,1))return FALSE;p++;}
 }
 if(hits<10||hits>100)return FALSE;
 if(!rpt_append_raw(out,cap,&used,src+end,n-end))return FALSE;
 *outn=used;return TRUE;
}
static BOOL rpt_patch_ao_proof(const char*src,DWORD n,char*out,DWORD cap,DWORD*outn){
 const char*f1="define void @\"\\01?PTRGSFinal@@YAXXZ\"()";
 const char*f2="define void @\"\\01?PTRGSWithSERFinal@@YAXXZ\"()";
 char*a=(char*)rpt_alloc(cap),*b=(char*)rpt_alloc(cap);DWORD na,nb;
 BOOL ok=FALSE;
 if(!a||!b)goto done;
 if(!rpt_r15_one(src,n,f1,rpt_ao_final,sizeof(rpt_ao_final)/sizeof(rpt_ao_final[0]),a,cap,&na))goto done;
 if(!rpt_ao_retarget(a,na,f1,"\n; <label>:908",", %900 ]",b,cap,&nb))goto done;
 if(!rpt_r15_one(b,nb,f2,rpt_ao_ser,sizeof(rpt_ao_ser)/sizeof(rpt_ao_ser[0]),a,cap,&na))goto done;
 if(!rpt_ao_retarget(a,na,f2,"\n; <label>:910",", %902 ]",out,cap,outn))goto done;
 ok=TRUE;
 done:if(a)rpt_free(a);if(b)rpt_free(b);return ok;
}


static BOOL rpt_patch_ir(const char*src,DWORD n,char*out,DWORD cap,DWORD*outn){
 char*base=(char*)rpt_alloc(cap);DWORD nb=0;BOOL ok=FALSE;
 if(!base)return FALSE;
 if(rpt_patch_ir_c17_base(src,n,base,cap,&nb))ok=rpt_patch_ao_proof(base,nb,out,cap,outn);
 rpt_free(base);return ok;
}

static DWORD rpt_fourcc(char a,char b,char c,char d){return ((DWORD)(BYTE)a)|((DWORD)(BYTE)b<<8)|((DWORD)(BYTE)c<<16)|((DWORD)(BYTE)d<<24);}
static BOOL rpt_container_part(const BYTE* c,DWORD n,DWORD fourcc,const BYTE** out,DWORD* outn){
    DWORD count,i,off,sz;if(!c||n<32||c[0]!='D'||c[1]!='X'||c[2]!='B'||c[3]!='C'||rd32(c+24)!=n)return FALSE;
    count=rd32(c+28);if(count==0||count>64||32UL+count*4UL>n)return FALSE;
    for(i=0;i<count;i++){off=rd32(c+32+i*4);if(off>n-8)return FALSE;sz=rd32(c+off+4);if(sz>n-off-8)return FALSE;if(rd32(c+off)==fourcc){*out=c+off+8;*outn=sz;return TRUE;}}
    return FALSE;
}
static BOOL rpt_digest_nonzero(const BYTE* c,DWORD n){DWORD i;if(!c||n<20)return FALSE;for(i=4;i<20;i++)if(c[i])return TRUE;return FALSE;}
static DWORD rpt_align4(DWORD n){return (n+3UL)&~3UL;}
static BOOL rpt_compose_original_container(const BYTE* original,DWORD on,const BYTE* assembled,DWORD an,BYTE** out,DWORD* outn){
    DWORD oc,i,oo,of,os,total,cursor,sz,j;const BYTE* hp=NULLPTR;const BYTE* dp=NULLPTR;DWORD hn=0,dn=0;BYTE* b;DWORD hashSeen=0,dxilSeen=0;
    if(!out||!outn)return FALSE;*out=NULLPTR;*outn=0;
    if(!original||!assembled||on<32||an<32)return FALSE;
    if(original[0]!='D'||original[1]!='X'||original[2]!='B'||original[3]!='C'||rd32(original+24)!=on)return FALSE;
    if(assembled[0]!='D'||assembled[1]!='X'||assembled[2]!='B'||assembled[3]!='C'||rd32(assembled+24)!=an)return FALSE;
    if(!rpt_container_part(assembled,an,rpt_fourcc('H','A','S','H'),&hp,&hn)||hn==0)return FALSE;
    if(!rpt_container_part(assembled,an,rpt_fourcc('D','X','I','L'),&dp,&dn)||dn==0)return FALSE;
    oc=rd32(original+28);if(oc==0||oc>64||32UL+oc*4UL>on)return FALSE;
    total=rpt_align4(32UL+oc*4UL);
    for(i=0;i<oc;i++){
        oo=rd32(original+32+i*4UL);if(oo>on-8)return FALSE;of=rd32(original+oo);os=rd32(original+oo+4);if(os>on-oo-8)return FALSE;
        if(of==rpt_fourcc('H','A','S','H')){sz=hn;hashSeen++;}
        else if(of==rpt_fourcc('D','X','I','L')){sz=dn;dxilSeen++;}
        else sz=os;
        if(total>0xffffffffUL-rpt_align4(8UL+sz))return FALSE;total+=rpt_align4(8UL+sz);
    }
    if(hashSeen!=1||dxilSeen!=1||total<65536UL||total>1048576UL)return FALSE;
    b=(BYTE*)rpt_alloc(total);if(!b)return FALSE;for(i=0;i<total;i++)b[i]=0;
    for(i=0;i<4;i++)b[i]=original[i];for(i=4;i<20;i++)b[i]=0;for(i=20;i<24;i++)b[i]=original[i];wr32(b+24,total);wr32(b+28,oc);
    cursor=rpt_align4(32UL+oc*4UL);
    for(i=0;i<oc;i++){
        oo=rd32(original+32+i*4UL);of=rd32(original+oo);os=rd32(original+oo+4);wr32(b+32+i*4UL,cursor);wr32(b+cursor,of);
        if(of==rpt_fourcc('H','A','S','H')){wr32(b+cursor+4,hn);for(j=0;j<hn;j++)b[cursor+8+j]=hp[j];sz=hn;}
        else if(of==rpt_fourcc('D','X','I','L')){wr32(b+cursor+4,dn);for(j=0;j<dn;j++)b[cursor+8+j]=dp[j];sz=dn;}
        else{wr32(b+cursor+4,os);for(j=0;j<os;j++)b[cursor+8+j]=original[oo+8+j];sz=os;}
        cursor+=rpt_align4(8UL+sz);
    }
    if(cursor!=total){rpt_free(b);return FALSE;}*out=b;*outn=total;return TRUE;
}
static BOOL rpt_preserved_parts_match(const BYTE* original,DWORD on,const BYTE* rebuilt,DWORD rn){
    DWORD oc,rc,i,oo,ro,os,rs,of,rf;if(!original||!rebuilt||on<32||rn<32)return FALSE;oc=rd32(original+28);rc=rd32(rebuilt+28);if(oc!=rc||oc==0||oc>64)return FALSE;
    if(32UL+oc*4UL>on||32UL+rc*4UL>rn)return FALSE;
    for(i=0;i<oc;i++){
        oo=rd32(original+32+i*4);ro=rd32(rebuilt+32+i*4);if(oo>on-8||ro>rn-8)return FALSE;
        of=rd32(original+oo);rf=rd32(rebuilt+ro);os=rd32(original+oo+4);rs=rd32(rebuilt+ro+4);if(of!=rf)return FALSE;
        if(os>on-oo-8||rs>rn-ro-8)return FALSE;
        if(of==rpt_fourcc('H','A','S','H')||of==rpt_fourcc('D','X','I','L'))continue;
        if(os!=rs)return FALSE;{DWORD j;for(j=0;j<os;j++)if(original[oo+8+j]!=rebuilt[ro+8+j])return FALSE;}
    }
    return TRUE;
}


/* Fail closed unless the FINAL DXC validated container disassembles to
 * both protected exports, with our two precise constant-buffer loads in
 * EACH export. A valid DXBC checksum alone proves neither export survived. */
static BOOL rpt_verified_final_export(const char*ir,DWORD length,const char*fn){
 DWORD start=rpt_find(ir,length,fn,0,length),end,other,p;
 if(start==0xffffffffUL)return FALSE;
 other=rpt_find(ir,length,fn,start+1,length);
 if(other!=0xffffffffUL)return FALSE; /* no duplicate protected definition */
 end=rpt_find(ir,length,"\n}",start,length);
 if(end==0xffffffffUL)return FALSE;
 p=rpt_find(ir,length,", i32 33)",start,end);
 if(p==0xffffffffUL)return FALSE;
 p=rpt_find(ir,length,", i32 56)",start,end);
 if(p==0xffffffffUL)return FALSE;
 p=rpt_find(ir,length,"%rpt_p2_t1_choose2",start,end);
 if(p==0xffffffffUL)return FALSE;
 if(rpt_find(ir,length,"%rpt_p2_apply",start,end)==0xffffffffUL)return FALSE;
 if(rpt_find(ir,length,"%rpt_p2_loop_choose2",start,end)==0xffffffffUL)return FALSE;
 if(rpt_find(ir,length,"%rpt_out",start,end)!=0xffffffffUL)return FALSE;
 /* AO proof must exist in BOTH validated exports, not just pre-assembly text. */
 if(rpt_find(ir,length,"%rpt_ao_payload",start,end)==0xffffffffUL ||
    rpt_find(ir,length,"%rpt_ao_vis",start,end)==0xffffffffUL ||
    rpt_find(ir,length,"%rpt_ao_indirect",start,end)==0xffffffffUL ||
    rpt_find(ir,length,"%rpt_ao_color2",start,end)==0xffffffffUL)return FALSE;
 return TRUE;
}
/* One signed container, no production game writes. The runtime owner is
 * responsible for holding output for the full DXR state-object lifetime. */
#include "cr01_pt_shared_dxil.h"
IMP int WINAPI VirtualProtect(void*,size_t,DWORD,DWORD*);
int cr01_build_shared_dxil(const uint8_t*original,size_t native_size,
                           CR01DxcCreate create,const uint8_t**out,size_t*out_size){
 IDxcLibrary*lib=0;IDxcCompiler*comp=0;IDxcAssembler*assem=0;IDxcValidator*validator=0;
 IDxcBlobEncoding*srcblob=0,*dis=0,*irblob=0,*composedblob=0,*verified_ir=0;
 IDxcOperationResult*op=0,*valop=0;
 IDxcBlob*assembled=0,*validated=0;
 BYTE*composed=0,*copy=0;char*patched=0;
 DWORD patchedn=0,composedn=0,disn=0,resultn=0,old=0;
 HRESULT hr,status;const char*distxt=0;const BYTE*apart=0;SIZE_T disbytes=0;
 DWORD apartn=0,cap=0;
 int success=0;
 if(!out||!out_size)return 0;*out=0;*out_size=0;
 if(!original||native_size!=231144u||!create||
   original[0]!='D'||original[1]!='X'||original[2]!='B'||original[3]!='C'||
   rd32(original+24)!=(DWORD)native_size)return 0;
 hr=create(&RPT_CLSID_DxcLibrary,&RPT_IID_IDxcLibrary,(LPVOID*)&lib);
 if(hr<0||!lib)goto done;
 hr=create(&RPT_CLSID_DxcCompiler,&RPT_IID_IDxcCompiler,(LPVOID*)&comp);
 if(hr<0||!comp)goto done;
 hr=create(&RPT_CLSID_DxcAssembler,&RPT_IID_IDxcAssembler,(LPVOID*)&assem);
 if(hr<0||!assem)goto done;
 hr=create(&RPT_CLSID_DxcValidator,&RPT_IID_IDxcValidator,(LPVOID*)&validator);
 if(hr<0||!validator)goto done;
 hr=lib->lpVtbl->CreateBlobWithEncodingFromPinned(lib,original,(DWORD)native_size,DXC_CP_ACP,&srcblob);
 if(hr<0||!srcblob)goto done;
 hr=comp->lpVtbl->Disassemble(comp,(IDxcBlob*)srcblob,&dis);
 if(hr<0||!dis)goto done;
 distxt=(const char*)((IDxcBlob*)dis)->lpVtbl->GetBufferPointer((IDxcBlob*)dis);
 disbytes=((IDxcBlob*)dis)->lpVtbl->GetBufferSize((IDxcBlob*)dis);
 if(!distxt||disbytes<1024u||disbytes>(SIZE_T)0xffffffffu-131072u)goto done;
 disn=(DWORD)disbytes;while(disn&&distxt[disn-1]==0)disn--;
 if(disn<1024u)goto done;
 cap=disn+131072u;patched=(char*)rpt_alloc(cap);
 if(!patched||!rpt_patch_ir(distxt,disn,patched,cap,&patchedn))goto done;
 hr=lib->lpVtbl->CreateBlobWithEncodingFromPinned(lib,patched,patchedn,DXC_CP_UTF8,&irblob);
 if(hr<0||!irblob)goto done;
 hr=assem->lpVtbl->AssembleToContainer(assem,(IDxcBlob*)irblob,&op);
 if(hr<0||!op)goto done;
 status=E_FAIL;hr=op->lpVtbl->GetStatus(op,&status);
 if(hr<0||status<0)goto done;
 hr=op->lpVtbl->GetResult(op,&assembled);
 if(hr<0||!assembled)goto done;
 apart=(const BYTE*)assembled->lpVtbl->GetBufferPointer(assembled);
 disbytes=assembled->lpVtbl->GetBufferSize(assembled);
 if(!apart||disbytes<65536u||disbytes>1048576u||
    rd32(apart+24)!=(DWORD)disbytes)goto done;
 apartn=(DWORD)disbytes;
 if(!rpt_compose_original_container(original,(DWORD)native_size,
    apart,apartn,&composed,&composedn))goto done;
 if(!rpt_preserved_parts_match(original,(DWORD)native_size,composed,composedn))goto done;
 hr=lib->lpVtbl->CreateBlobWithEncodingFromPinned(lib,composed,composedn,DXC_CP_ACP,&composedblob);
 if(hr<0||!composedblob)goto done;
 hr=validator->lpVtbl->Validate(validator,(IDxcBlob*)composedblob,1,&valop);
 if(hr<0||!valop)goto done;
 status=E_FAIL;hr=valop->lpVtbl->GetStatus(valop,&status);
 if(hr<0||status<0)goto done;
 hr=valop->lpVtbl->GetResult(valop,&validated);
 if(hr<0||!validated)goto done;
 apart=(const BYTE*)validated->lpVtbl->GetBufferPointer(validated);
 disbytes=validated->lpVtbl->GetBufferSize(validated);
 if(!apart||disbytes<65536u||disbytes>1048576u)goto done;
 resultn=(DWORD)disbytes;
 if(apart[0]!='D'||apart[1]!='X'||apart[2]!='B'||apart[3]!='C'||
    rd32(apart+24)!=resultn||!rpt_digest_nonzero(apart,resultn)||
    !rpt_preserved_parts_match(original,(DWORD)native_size,apart,resultn))goto done;
 /* Verify BOTH protected functions AFTER assembly, signing and validation. */
 hr=comp->lpVtbl->Disassemble(comp,validated,&verified_ir);
 if(hr<0||!verified_ir)goto done;
 {
  const char *final_ir=(const char*)((IDxcBlob*)verified_ir)->lpVtbl->GetBufferPointer((IDxcBlob*)verified_ir);
  SIZE_T irlen=((IDxcBlob*)verified_ir)->lpVtbl->GetBufferSize((IDxcBlob*)verified_ir);
  if(!final_ir||irlen<1024u||irlen>0xffffffffu)goto done;
  if(!rpt_verified_final_export(final_ir,(DWORD)irlen,"define void @\"\\01?PTRGSFinal@@YAXXZ\"()") ||
     !rpt_verified_final_export(final_ir,(DWORD)irlen,"define void @\"\\01?PTRGSWithSERFinal@@YAXXZ\"()"))goto done;
 }
 /* The two protected exports must remain within ONE validated container.
    Higher-level runtime registration confirms actual GPU state-object creation. */
 copy=(BYTE*)rpt_alloc(resultn);
 if(!copy)goto done;
 for(DWORD i=0;i<resultn;i++)copy[i]=apart[i];
 if(!VirtualProtect(copy,resultn,0x02,&old))goto done; /* PAGE_READONLY */
 *out=copy;*out_size=resultn;copy=0;success=1;
 done:
 if(copy)rpt_free(copy);
 if(verified_ir)((IDxcBlob*)verified_ir)->lpVtbl->Release((IDxcBlob*)verified_ir);
 if(validated)validated->lpVtbl->Release(validated);
 if(valop)valop->lpVtbl->Release(valop);
 if(composedblob)((IDxcBlob*)composedblob)->lpVtbl->Release((IDxcBlob*)composedblob);
 if(assembled)assembled->lpVtbl->Release(assembled);
 if(op)op->lpVtbl->Release(op);
 if(irblob)((IDxcBlob*)irblob)->lpVtbl->Release((IDxcBlob*)irblob);
 if(dis)((IDxcBlob*)dis)->lpVtbl->Release((IDxcBlob*)dis);
 if(srcblob)((IDxcBlob*)srcblob)->lpVtbl->Release((IDxcBlob*)srcblob);
 if(validator)validator->lpVtbl->Release(validator);
 if(assem)assem->lpVtbl->Release(assem);
 if(comp)comp->lpVtbl->Release(comp);
 if(lib)lib->lpVtbl->Release(lib);
 if(composed)rpt_free(composed);
 if(patched)rpt_free(patched);
 return success;
}

/* Freestanding Windows x64 target without CRT. The optimizer may lower small
 * bulk initialization/copy loops from the preserved LC44 implementation to
 * memcpy/memset; supply bounded local implementations, no CRT dependency. */
__attribute__((optnone)) void* memcpy(void*dst,const void*src,size_t n){
 uint8_t*d=(uint8_t*)dst;const uint8_t*s=(const uint8_t*)src;
 for(size_t i=0;i<n;i++)d[i]=s[i];return dst;
}
__attribute__((optnone)) void* memset(void*dst,int byte,size_t n){
 uint8_t*d=(uint8_t*)dst;for(size_t i=0;i<n;i++)d[i]=(uint8_t)byte;return dst;
}
