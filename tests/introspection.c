#include <assert.h>
#include <stdio.h>
#include <elf.h>
#include <link.h>

extern __attribute__((visibility("hidden"))) ElfW(Ehdr) __ehdr_start;

int main()
{
  const ElfW(Ehdr) ehdr = __ehdr_start;
  const ElfW(Phdr) *phdr = (ElfW(Phdr) *) ((size_t) &__ehdr_start + __ehdr_start.e_phoff);

  const ElfW(Dyn) *dyn = _DYNAMIC;
  const ElfW(Dyn) *rpath = NULL;
  const ElfW(Dyn) *runpath = NULL;
  const char *strtab = NULL;

  for (; dyn->d_tag != DT_NULL; ++dyn) {
    if (dyn->d_tag == DT_RPATH) {
      rpath = dyn;
    } else if (dyn->d_tag == DT_RUNPATH) {
      runpath = dyn;
    } else if (dyn->d_tag == DT_STRTAB) {
      strtab = (const char *)dyn->d_un.d_val;
    }
  }

  assert(strtab != NULL);

  printf("ehdr.e_phnum=%d", (int) ehdr.e_phnum);
  printf(", phdr[0].p_type=%d", (int) phdr[0].p_type);

  if (rpath != NULL) {
    printf(", RPATH=%s", strtab + rpath->d_un.d_val);
  } else if (runpath != NULL) {
    printf(", RUNPATH=%s", strtab + runpath->d_un.d_val);
  }

  printf("\n");

  return 0;
}
