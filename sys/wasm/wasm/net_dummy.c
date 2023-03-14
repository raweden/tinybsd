
// moved from misc_dummy.c

epoch_t net_epoch_preempt;
uint32_t _rt_numfibs;

struct ifnet *ifnet_byindexgen(uint16_t idx, uint16_t gen)
{
	return NULL;
}

void if_ref(struct ifnet *ifp)
{

}

void if_rele(struct ifnet *ifp)
{

}

int	ifioctl(struct socket *, u_long, caddr_t, struct thread *)
{
	return 0;
}

const char *if_name(void *ifp)
{
	return "emsc";
}

int if_getindex(const void *d)
{
	return 0;
}

int if_getidxgen(const void *d)
{
	
	return 0;
}

int if_getflags(const void *d)
{
	return 0;
}

int if_snd_tag_alloc(void *ifp, void *params, void **mstp)
{
	return 0;
}

void netisr_sched_poll(void)
{

}

int	 rn_inithead(void **arg1, int arg2)
{
	return 0;
}

int	 rn_detachhead(void **arg1)
{
	return 0;
}

int	rtioctl_fib(u_long, caddr_t, u_int)
{
	return 0;
}

int	sys_setfib(struct thread *, struct setfib_args *)
{
	return 0;
}