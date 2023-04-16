
#include <sys/types.h>
#if 0
void __libc_map_stacks_exec(void)
{

}
#endif

#include <pthread.h>
#include <pthread_np.h>

typedef int dlfunc_t;

static const char sorry[] = "Service unavailable";

int _rtld_get_stack_prot(void)
{
    return 0;
}

void _rtld_error(const char *fmt __unused, ...)
{

}

int dlclose(void *handle)
{
    return 0;
}

void *dlopen(const char *name, int mode)
{
    return NULL;
}

dlfunc_t dlfunc(void *handle, const char *name)
{
    return 0;
}

char *dlerror(void)
{
    return (char *)sorry;
}

// dummy for libthr

void ___pthread_cleanup_push_imp(void (*)(void *), void *, struct _pthread_cleanup_info *) __attribute__((alias("__pthread_cleanup_push_imp")));
void __pthread_cleanup_push_imp(void (*routine)(void *), void *arg, struct _pthread_cleanup_info *info)
{

}

void ___pthread_cleanup_pop_imp(int) __attribute__((alias("__pthread_cleanup_pop_imp")));
void __pthread_cleanup_pop_imp(int execute)
{

}

// __pthread_cleanup_push_imp
// __pthread_cleanup_pop_imp

int _pthread_mutex_lock(pthread_mutex_t * __mutex)
{
    return 0;
}

int _pthread_mutex_unlock(pthread_mutex_t * __mutex)
{
    return 0;
}

int _pthread_once(pthread_once_t *once_control, void (*init_routine)(void))
{
    return 0;
}

int _pthread_main_np(void)
{
    return 0;
}

void *pthread_getspecific(pthread_key_t) __attribute__((alias("_pthread_getspecific")));
void *_pthread_getspecific(pthread_key_t userkey)
{
    return NULL;
}

int pthread_setspecific(pthread_key_t, const void *) __attribute__((alias("_pthread_setspecific")));
int _pthread_setspecific(pthread_key_t userkey, const void *value)
{
    return 0;
}

int pthread_key_create(pthread_key_t *, void (*)(void *)) __attribute__((alias("_pthread_key_create")));
int _pthread_key_create(pthread_key_t *key, void (*destructor)(void *))
{
    return 0;
}

int _pthread_rwlock_unlock(pthread_rwlock_t *rwlock)
{
    return 0;
}

pthread_t _pthread_self(void)
{
    return NULL;
}

int _pthread_rwlock_rdlock(pthread_rwlock_t *rwlock)
{
    return 0;
}

int _pthread_rwlock_wrlock(pthread_rwlock_t *rwlock)
{
    return 0;
}

int _pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    return 0;
}

int _pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    return 0;
}

int _rtld_addr_phdr(const void *addr, struct dl_phdr_info *phdr_info_a)
{
    return 0;
}

struct tm *localtime_r (const time_t *__timer, struct tm *__tp)
{
    return NULL;
}

// _pthread_mutex_lock
// _pthread_mutex_unlock
// _pthread_once
// _pthread_main_np
// _pthread_getspecific
// _pthread_setspecific
// _pthread_key_create
// _pthread_rwlock_unlock
// _pthread_self
// pthread_setspecific
// pthread_getspecific
// __pthread_cleanup_push_imp
// __pthread_cleanup_pop_imp
// _pthread_rwlock_rdlock
// _pthread_rwlock_wrlock
// _pthread_mutex_destroy
// _pthread_mutex_trylock


int _nsyyparse(void)
{
    return 0;
}

//

long double strtold_l(const char * __restrict s, char ** __restrict sp, locale_t locale)
{
    return 0;
}

//

#include "../locale/xlocale_private.h"

#ifndef MONSPERYEAR
#define MONSPERYEAR (12)
#endif

#ifndef DAYSPERWEEK
#define DAYSPERWEEK (7)
#endif

struct lc_time_T {
	const char *	mon[MONSPERYEAR];
	const char *	month[MONSPERYEAR];
	const char *	wday[DAYSPERWEEK];
	const char *	weekday[DAYSPERWEEK];
	const char *	X_fmt;
	const char *	x_fmt;
	const char *	c_fmt;
	const char *	am;
	const char *	pm;
	const char *	date_fmt;
};

struct xlocale_time {
	struct xlocale_component header;
	char *buffer;
	struct lc_time_T locale;
};

struct xlocale_time __xlocale_global_time;

int __time_load_locale(const char *name)
{
    return 0;
}

void* __time_load(const char* name, locale_t loc)
{
    return NULL;
}

struct lc_time_T *__get_current_time_locale(locale_t loc)
{
    return NULL;
}

struct tm *gmtime(const time_t *)
{
    return NULL;
}

struct tm *localtime(const time_t *)
{
    return NULL;
}

time_t timegm(struct tm * const)
{
    return 0;
}

char *strptime(const char *arg1, const char *arg2, struct tm *arg3)
{
    return NULL;
}

size_t strftime(char * __restrict, size_t, const char * __restrict, const struct tm * __restrict)
{
    return 0;
}

#if 0
int strvisx(char *mbdst, const char *mbsrc, size_t len, int flags)
{
    return 0;
}
#endif

char *asctime(const struct tm *)
{
    return NULL;
}

time_t mktime(struct tm *)
{
    return 0;
}

char *ctime_r(const time_t *, char *)
{
    return NULL;
}