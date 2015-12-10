#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "erl_nif.h"

/*
 * Jumping Consistent Hash Algorithm. John Lamping, Eric Veach -- Google.
 * <a href="http://arxiv.org/ftp/arxiv/papers/1406/1406.2294.pdf"></a>
 */
static int32_t
jumping_hash (uint64_t key, int32_t num_buckets) {
  int64_t b = -1, j = 0;
  while (j < num_buckets) {
    b = j;
    key = key * 2862933555777941757ULL + 1;
    j = (b + 1) * (((double) (1LL << 31)) / ((double) ((key >> 33) + 1)));
  }
  return b;
}

static ERL_NIF_TERM
compute (ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  int64_t key;
  int32_t num_buckets, ret;

  if (!enif_get_uint64 (env, argv[0], &key) ||
      !enif_get_uint (env, argv[1], &num_buckets)) {
    return enif_make_badarg (env);
  }

  ret = jumping_hash (key, num_buckets);

  return enif_make_int (env, ret);
}

static ErlNifFunc nif_funcs[] = {
  {"compute", 2, compute}
};

ERL_NIF_INIT (toy_kv_jchash, nif_funcs, NULL, NULL, NULL, NULL);
