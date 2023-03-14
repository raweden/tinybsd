
/*
g_debugflags	 	-> geom_dump.c
g_trace
g_collectstats 		-> geom_kern.c
g_shutdown
g_notaste
g_init
g_down_td
g_up_td

g_ctl_init 			-> geom_ctl.c
*/

void g_destroy_bio(struct bio *)
{

}

struct bio *g_alloc_bio(void)
{
	
}

void g_waitidle(struct thread *td)
{

}

int g_access(struct g_consumer *cp, int nread, int nwrite, int nexcl)
{

}

void g_detach(struct g_consumer *cp)
{

}

void g_destroy_consumer(struct g_consumer *cp)
{

}

void g_reset_bio(struct bio *)
{

}

void g_dev_print(void)
{

}

struct g_provider *g_dev_getprovider(struct cdev *dev)
{

}

struct g_geom *g_new_geomf(struct g_class *mp, const char *fmt, ...)
{

}

struct g_consumer *g_new_consumer(struct g_geom *gp)
{

}

int g_attach(struct g_consumer *cp, struct g_provider *pp)
{

}

struct bio *g_new_bio(void)
{

}

void g_io_request(struct bio *bp, struct g_consumer *cp)
{

}

int g_waitfor_event(g_event_t *func, void *arg, int flag, ...)
{

}

int g_post_event(g_event_t *func, void *arg, int flag, ...)
{

}