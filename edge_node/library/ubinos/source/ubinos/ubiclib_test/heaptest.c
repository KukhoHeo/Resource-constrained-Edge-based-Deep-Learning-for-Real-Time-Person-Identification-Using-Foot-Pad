#include <ubinos/bsp/arch.h>
#include <ubinos/ubiclib_test.h>
#include <ubinos/ubiclib/heap.h>

#if (INCLUDE__UBINOS__UBICLIB_TEST == 1)
#if !(UBINOS__UBICLIB_TEST__EXCLUDE_HEAPTEST == 1)

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__USER00

/*
N = hist(Y,nbins)
Y = unifpdf(X,A,B)
R = unifrnd(A,B,m,n)
Y = exppdf(X,mu)
R = exprnd(mu,m,n)
Y = normpdf(X,mu,sigma)
R = normrnd(mu,sigma,m,n)

urx1 = unifrnd(0,1,1,4000);
urx2 = unifrnd(0,1,1,4000);
erx1 = exprnd(1,1,4000);
erx2 = exprnd(1,1,4000);
nrx1 = normrnd(0,1, 1, 4000);
nrx2 = normrnd(0,1, 1, 4000);

dlmwrite('urx1.txt', urx1, ',');
dlmwrite('urx2.txt', urx2, ',');
dlmwrite('erx1.txt', erx1, ',');
dlmwrite('erx2.txt', erx2, ',');
dlmwrite('nrx1.txt', nrx1, ',');
dlmwrite('nrx2.txt', nrx2, ',');

hist(urx1*2, 100);
mean(urx1*2)
hist(erx1, 100);
mean(erx1)
hist(nrx1*32+128, 100);
mean(nrx1*32+128)
 */

void ubiclib_test_heaptest(void *arg) {
	ubiclib_test_heaptest_param_t *params = arg;
	assert(params != NULL);

	void *		heap_ptr;
	heap_pt 	heap;
	edlist_t *	blist_p;

	int r;
	unsigned int h, i, j, ii, jj;
	unsigned int stepcount;
	unsigned int testcount;
	unsigned int fbcount;
	unsigned int firstfull;
	ubiclib_test_heaptest_result_pt result_p;
	ubiclib_test_heaptest_result_t  result_temp;

	unsigned int sizer, nsizer, rsizer;
	unsigned int sizea, nsizea, rsizea, sizea_temp;
	unsigned int count, ncount, rcount;
	unsigned int boh;
	unsigned int oh, noh, roh;
	unsigned int lifetime;
	unsigned int size;

	void * 			ptr;
	int				index;
	edlist_pt		blist;
	edlist_elmt_pt 	ble, ble_old;

	unsigned int	blist_ci;

	hrtick_t tick1, tick2, diff;
	unsigned int current;

	double etimea, etimef;											// execution time (allocation, free)
	double frag, fragi, frage;										// fragmentation (total, internal,external)
	double average, sdev;											// average, standard deviation
	unsigned int ssum  = 0;											// sum of size
	unsigned int csum  = 0;											// sum of count
	double esuma = 0, esum2a = 0, ebesta = UINT_MAX, eworsta = 0;	// allocation execution time: sum of ~, sum of square of ~, best ~, worst ~
	double esumf = 0, esum2f = 0, ebestf = UINT_MAX, eworstf = 0;	//       free execution time: sum of ~, sum of square of ~, best ~, worst ~
	double fsum  = 0, fsum2  = 0, fbest  = UINT_MAX, fworst  = 0;	//       total fragmentation: sum of ~, sum of square of ~, best ~, worst ~
	double fsumi = 0, fsum2i = 0, fbesti = UINT_MAX, fworsti = 0;	//    internal fragmentation: sum of ~, sum of square of ~, best ~, worst ~
	double fsume = 0, fsum2e = 0, fbeste = UINT_MAX, fworste = 0;	//    external fragmentation: sum of ~, sum of square of ~, best ~, worst ~
	unsigned int enoa = 0;											//  allocation execution time: count of ~
	unsigned int enof = 0;											//        free execution time: count of ~
	unsigned int fno  = 0;											//        total fragmentation: count of ~
	unsigned int fnoi = 0;											//     internal fragmentation: count of ~
	unsigned int fnoe = 0;											//     external fragmentation: count of ~
	double average_etimea, average_etimef;
	double average_frag, average_fragi, average_frage;

	blist_p = malloc(sizeof(edlist_t) * params->lifetime_max);
	assert(blist_p != NULL);

	heap_ptr = malloc(params->heapsize);
	assert(heap_ptr != NULL);

	result_p = malloc(sizeof(ubiclib_test_heaptest_result_t) * params->stepcount_max);
	assert(result_p != NULL);

	stepcount 		= 0;
	average_etimea	= 0;
	average_etimef	= 0;
	average_frag	= 0;
	average_fragi	= 0;
	average_frage	= 0;

	i = 0;
	j = 0;

//	printf( "  s-m: target size mean\n"
//			"r-s-m: real size mean\n"
//			"  b-c: block count mean when allocation fail\n"
//			"  a-c: allocation count\n"
//			" at-a: allocation execution time average\n"
//			" at-d: allocation execution time standard deviation\n"
//			" at-b: allocation execution time best\n"
//			" at-w: allocation execution time worst\n"
//			"  r-c: release count\n"
//			" rt-a: release execution time average\n"
//			" rt-d: release execution time standard deviation\n"
//			" rt-b: release execution time best\n"
//			" rt-w: release execution time worst\n"
//			" tf-a: total fragmentation average\n"
//			" tf-d: total fragmentation standard deviation\n"
//			" tf-b: total fragmentation best\n"
//			" tf-w: total fragmentation worst\n"
//			" if-a: internal fragmentation average\n"
//			" if-d: internal fragmentation standard deviation\n"
//			" if-b: internal fragmentation best\n"
//			" if-w: internal fragmentation worst\n"
//			" ef-a: external fragmentation average\n"
//			" ef-d: external fragmentation standard deviation\n"
//			" ef-b: external fragmentation best\n"
//			" ef-w: external fragmentation worst\n");

	printf("      s-m,    r-s-m,      b-c,      a-c,     at-a,     at-d,     at-b,     at-w,      r-c,     rt-a,     rt-d,     rt-b,     rt-w,   tf-a,   tf-d,   tf-b,   tf-w,   if-a,   if-d,   if-b,   if-w,   ef-a,   ef-d,   ef-b,   ef-w\n");


	for (h = params->mean_min;; h += params->mean_step) {
		//logm_setlevel(LOGM_CATEGORY__HEAP,		LOGM_LEVEL__ERROR);


		r = heap_create_ext(	&heap, (unsigned int) heap_ptr, params->heapsize,
				params->algorithm_type, UBINOS__UBICLIB__HEAP_LOCK_TYPE__NONE, params->m, 0,
				params->algorithm_type, UBINOS__UBICLIB__HEAP_LOCK_TYPE__NONE, params->m, 0			);

		//logm_setlevel(LOGM_CATEGORY__HEAP,		LOGM_LEVEL__DEBUG);

		if (0 != r) {
			logme("fail at heap_create_ext");
			bsp_abortsystem();
		}

		heap_getblockoverhead(heap, &boh);

		for (ii=0; ii<params->lifetime_max; ii++) {
			edlist_init(&blist_p[ii]);
		}

		firstfull = 0;

		ssum = 0;   // size sum
		csum = 0;   // count sum

		esuma = 0;
		esum2a = 0;
		ebesta = UINT_MAX;
		eworsta = 0;
		enoa = 0;

		esumf = 0;
		esum2f = 0;
		ebestf = UINT_MAX;
		eworstf = 0;
		enof = 0;

		fsum = 0;
		fsum2 = 0;
		fbest = UINT_MAX;
		fworst = 0;
		fno = 0;

		fsumi = 0;
		fsum2i = 0;
		fbesti = UINT_MAX;
		fworsti = 0;
		fnoi = 0;

		fsume = 0;
		fsum2e = 0;
		fbeste = UINT_MAX;
		fworste = 0;
		fnoe = 0;

		if (0 != r) {
			logme("fail at heap_create_ext");
			break;
		}

		testcount = 0;
		blist_ci = 0;

		for (;;) {
			switch (params->lifetime_type) {
			case UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM:
				lifetime = (unsigned int) (((params->lifetime_p[j] * 2    ) / 2 * (params->lifetime_max - 0.01)) + 1);
				break;
			case UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__EXPONENTIAL:
				lifetime = (unsigned int) (((params->lifetime_p[j]        ) / 8 * (params->lifetime_max - 0.01)) + 1);
				break;
			case UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__NORMAL:
				lifetime = (unsigned int) (((params->lifetime_p[j] / 4 + 1) / 2 * (params->lifetime_max - 0.01)) + 1);
				break;
			default:
				lifetime = 0;
				bsp_abortsystem();
			}
			if (0 >= lifetime || params->lifetime_max <= lifetime) {
				j++;
				if (params->data_length <= j) {
					j=0;
				}
				continue;
			}

			switch (params->size_type) {
			case UBICLIB_TEST_HEAPTEST_SIZE_TYPE__UNIFORM:
				size = (unsigned int) ((params->size_p[i] * 2    ) * h);
				break;
			case UBICLIB_TEST_HEAPTEST_SIZE_TYPE__EXPONENTIAL:
				size = (unsigned int) ((params->size_p[i]        ) * h);
				break;
			case UBICLIB_TEST_HEAPTEST_SIZE_TYPE__NORMAL:
				size = (unsigned int) ((params->size_p[i] / 4 + 1) * h);
				break;
			default:
				size = 0;
				bsp_abortsystem();
			}
			size	 = MEM_ALIGN(size);
			if (params->size_min >= size || params->size_max <= size) {
				i++;
				if (params->data_length <= i) {
					i=0;
				}
				continue;
			}

			logmfd("malloc: %8d, %8d", size, lifetime);

			if (1 == params->critlock) {
				intr_disable(AT91C_ID_SYS);
			}

			ubik_hrtick_reset();
			ubik_hrtick_gettick(&tick1);
			ptr = heap_mallocn(heap, size - boh);
			ubik_hrtick_gettick(&tick2);

			if (1 == params->critlock) {
				intr_enable(AT91C_ID_SYS);
			}

			if (NULL != ptr) {
				logmd("    success");

				if (1 == params->blockcheck) {
					heap_checkblockboundaryall(heap);
					memset(ptr, 0xff, size - boh);
					heap_checkblockboundaryall(heap);
				}

				index = (blist_ci + lifetime) % params->lifetime_max;
				blist = &blist_p[index];
				edlist_insertprev(edlist_elmt_pt, link, blist, NULL, (edlist_elmt_pt) ptr);

				j++;
				if (params->data_length <= j) {
					j=0;
				}

				i++;
				if (params->data_length <= i) {
					i=0;
				}

				if (1 == params->skipfirsfull && 0 == firstfull) {
					continue;
				}

				ubik_hrtick_gettickdiff(&tick1, &tick2, &diff);
				ubik_hrtick_hrticktotimens(&diff, &current);
				etimea = (double) current;
				esuma  += (etimea);
				esum2a += (etimea * etimea);
				if (ebesta > etimea) {
					ebesta = etimea;
				}
				if (eworsta < etimea) {
					eworsta = etimea;
				}
				enoa++;
				ssum += size;

				continue;
			}
			else {
				if (0 == firstfull) {
					firstfull = 1;
				}
			}

			logmd("    fail");

			heap_getrequestedsize_ext(heap, &sizer, &nsizer, &rsizer);
			heap_getallocatedsize_ext(heap, &sizea, &nsizea, &rsizea);
			heap_getallocatedcount_ext(heap, &count, &ncount, &rcount);
			noh = ncount * boh;
			roh = rcount * boh;
			oh  = noh + roh;

			for (;;) {
				do {
					blist_ci = (blist_ci + 1) % params->lifetime_max;
					blist = &blist_p[blist_ci];
				} while (0 == blist->count);

				ble_old = NULL;
				for (;;) {
					ble = edlist_head(edlist_elmt_pt, link, blist);
					if (NULL == ble) {
						break;
					}
					if (ble_old == ble) {
						bsp_abortsystem();
					}
					ble_old = ble;
					edlist_remove(edlist_elmt_pt, link, ble);

					if (1 == params->critlock) {
						intr_disable(AT91C_ID_SYS);
					}
					ubik_hrtick_reset();
					ubik_hrtick_gettick(&tick1);
					heap_free(heap, ble);
					ubik_hrtick_gettick(&tick2);
					if (1 == params->critlock) {
						intr_enable(AT91C_ID_SYS);
					}

					if (1 == params->blockcheck) {
						heap_checkblockboundaryall(heap);
					}

					ubik_hrtick_gettickdiff(&tick1, &tick2, &diff);
					ubik_hrtick_hrticktotimens(&diff, &current);
					etimef = (double) current;
					esumf  += (etimef);
					esum2f += (etimef * etimef);
					if (ebestf > etimef) {
						ebestf = etimef;
					}
					if (eworstf < etimef) {
						eworstf = etimef;
					}
					enof++;
					//heap_getallocatedsize(heap, &sizea_temp);
					//printf("==> heapsize = %d, sizea_temp = %d, free size = %d (%d %%)\n", heapsize, sizea_temp, heapsize - sizea_temp, 100 * (heapsize - sizea_temp) / heapsize);
				}

				heap_getallocatedsize(heap, &sizea_temp);
				if ((params->heapsize / 2) < (params->heapsize - sizea_temp)) {
					break;
				}
			}

			if ((params->heapsize - sizea) < (size)) {
				i++;
				if (params->data_length <= i) {
					i=0;
				}
				continue;
			}

			testcount++;

			csum += count;

			fragi = ((double) sizea - (sizer + oh)) / ((double) sizea);
			frage = ((double) params->heapsize - sizea) / params->heapsize;
			if (0 > frage) {
				frage = 0;
			}
			frag = fragi + frage - fragi * frage;

			fsum  += (frag);
			fsum2 += (frag * frag);
			if (fbest > frag) {
				fbest = frag;
			}
			if (fworst < frag) {
				fworst = frag;
			}
			fno++;

			fsumi += (fragi);
			fsum2i += (fragi * fragi);
			if (fbesti > fragi) {
				fbesti = fragi;
			}
			if (fworsti < fragi) {
				fworsti = fragi;
			}
			fnoi++;

			fsume  += (frage);
			fsum2e += (frage * frage);
			if (fbeste > frage) {
				fbeste = frage;
			}
			if (fworste < frage) {
				fworste = frage;
			}
			fnoe++;

			if (params->testcount_max <= testcount) {
				break;
			}

			//heap_getallocatedsize(heap, &sizea_temp);
			//printf("\n==> heapsize = %d, sizea_temp = %d, free size = %d (%d %%)\n\n", heapsize, sizea_temp, heapsize - sizea_temp, 100 * (heapsize - sizea_temp) / heapsize);
		}

		for (blist_ci=0; blist_ci<params->lifetime_max; blist_ci++) {
			blist = &blist_p[blist_ci];

			ble_old = NULL;
			for (;;) {
				ble = edlist_head(edlist_elmt_pt, link, blist);
				if (NULL == ble) {
					break;
				}
				if (ble_old == ble) {
					bsp_abortsystem();
				}
				ble_old = ble;
				edlist_remove(edlist_elmt_pt, link, ble);

				if (1 == params->critlock) {
					intr_disable(AT91C_ID_SYS);
				}
				ubik_hrtick_reset();
				ubik_hrtick_gettick(&tick1);
				heap_free(heap, ble);
				ubik_hrtick_gettick(&tick2);
				if (1 == params->critlock) {
					intr_enable(AT91C_ID_SYS);
				}

				if (1 == params->blockcheck) {
					heap_checkblockboundaryall(heap);
				}

				if (1 != params->skipfirsfull) {
					ubik_hrtick_gettickdiff(&tick1, &tick2, &diff);
					ubik_hrtick_hrticktotimens(&diff, &current);
					etimef = (double) current;
					esumf  += (etimef);
					esum2f += (etimef * etimef);
					if (ebestf > etimef) {
						ebestf = etimef;
					}
					if (eworstf < etimef) {
						eworstf = etimef;
					}
					enof++;
				}
			}
		}

		result_p[stepcount].sm 		= h;
		result_p[stepcount].rsm		= (ssum / enoa);
		result_p[stepcount].bc		= (csum / params->testcount_max);

		average = esuma / enoa;
		sdev = (esum2a / enoa) - (average * average);
		sdev = sqrt(sdev);
		result_p[stepcount].ac		= enoa;
		result_p[stepcount].ata		= average;
		result_p[stepcount].atd		= sdev;
		result_p[stepcount].atb		= ebesta;
		result_p[stepcount].atw		= eworsta;
		average_etimea	+= average;

		average = esumf / enof;
		sdev = (esum2f / enof) - (average * average);
		sdev = sqrt(sdev);
		result_p[stepcount].rc		= enof;
		result_p[stepcount].rta		= average;
		result_p[stepcount].rtd		= sdev;
		result_p[stepcount].rtb		= ebestf;
		result_p[stepcount].rtw		= eworstf;
		average_etimef	+= average;

		average = fsum / fno;
		sdev = (fsum2 / fno) - (average * average);
		sdev = sqrt(sdev);
		result_p[stepcount].tfa		= average;
		result_p[stepcount].tfd		= sdev;
		result_p[stepcount].tfb		= fbest;
		result_p[stepcount].tfw		= fworst;
		average_frag	+= average;

		average = fsumi / fnoi;
		sdev = (fsum2i / fnoi) - (average * average);
		sdev = sqrt(sdev);
		result_p[stepcount].ifa		= average;
		result_p[stepcount].ifd		= sdev;
		result_p[stepcount].ifb		= fbesti;
		result_p[stepcount].ifw		= fworsti;
		average_fragi	+= average;

		average = fsume / fnoe;
		sdev = (fsum2e / fnoe) - (average * average);
		sdev = sqrt(sdev);
		result_p[stepcount].efa		= average;
		result_p[stepcount].efd		= sdev;
		result_p[stepcount].efb		= fbeste;
		result_p[stepcount].efw		= fworste;
		average_frage	+= average;

		heap_getfreeblockcount(heap, &fbcount);
		if (1 != fbcount) {
			logmfe("free block count (%d) is wrong.", fbcount);
			heap_printheapinfo(heap);
			bsp_abortsystem();
		}

		if (0 == count) {
			logmfe("block count (%d) is wrong.", count);
			heap_printheapinfo(heap);
			bsp_abortsystem();
		}

		heap_delete(&heap);

		stepcount++;

		if (params->stepcount_max <= stepcount) {
			break;
		}
	}

	for (ii=0; ii<params->stepcount_max; ii++) {
		for (jj=1; jj<(params->stepcount_max-ii); jj++) {
			if (result_p[jj-1].rsm > result_p[jj].rsm) {
				result_temp 	= result_p[jj-1];
				result_p[jj-1] 	= result_p[jj];
				result_p[jj] 	= result_temp;
			}
		}
	}
	for (ii=0; ii<params->stepcount_max; ii++) {
		printf(" %8d, %8d, %8d",
				(unsigned int) (result_p[ii].sm        ),
				(unsigned int) (result_p[ii].rsm       ),
				(unsigned int) (result_p[ii].bc        ));
		printf(", %8d, %8d, %8d, %8d, %8d",
				(unsigned int) (result_p[ii].ac        ),
				(unsigned int) (result_p[ii].ata / 1000),
				(unsigned int) (result_p[ii].atd / 1000),
				(unsigned int) (result_p[ii].atb / 1000),
				(unsigned int) (result_p[ii].atw / 1000));
		printf(", %8d, %8d, %8d, %8d, %8d",
				(unsigned int) (result_p[ii].rc        ),
				(unsigned int) (result_p[ii].rta / 1000),
				(unsigned int) (result_p[ii].rtd / 1000),
				(unsigned int) (result_p[ii].rtb / 1000),
				(unsigned int) (result_p[ii].rtw / 1000));
		printf(", %3d.%02d, %3d.%02d, %3d.%02d, %3d.%02d",
				(unsigned int) (result_p[ii].tfa * 100), ((unsigned int) (result_p[ii].tfa * 10000)) % 100,
				(unsigned int) (result_p[ii].tfd * 100), ((unsigned int) (result_p[ii].tfd * 10000)) % 100,
				(unsigned int) (result_p[ii].tfb * 100), ((unsigned int) (result_p[ii].tfb * 10000)) % 100,
				(unsigned int) (result_p[ii].tfw * 100), ((unsigned int) (result_p[ii].tfw * 10000)) % 100
			);
		printf(", %3d.%02d, %3d.%02d, %3d.%02d, %3d.%02d",
				(unsigned int) (result_p[ii].ifa * 100), ((unsigned int) (result_p[ii].ifa * 10000)) % 100,
				(unsigned int) (result_p[ii].ifd * 100), ((unsigned int) (result_p[ii].ifd * 10000)) % 100,
				(unsigned int) (result_p[ii].ifb * 100), ((unsigned int) (result_p[ii].ifb * 10000)) % 100,
				(unsigned int) (result_p[ii].ifw * 100), ((unsigned int) (result_p[ii].ifw * 10000)) % 100
			);
		printf(", %3d.%02d, %3d.%02d, %3d.%02d, %3d.%02d",
				(unsigned int) (result_p[ii].efa * 100), ((unsigned int) (result_p[ii].efa * 10000)) % 100,
				(unsigned int) (result_p[ii].efd * 100), ((unsigned int) (result_p[ii].efd * 10000)) % 100,
				(unsigned int) (result_p[ii].efb * 100), ((unsigned int) (result_p[ii].efb * 10000)) % 100,
				(unsigned int) (result_p[ii].efw * 100), ((unsigned int) (result_p[ii].efw * 10000)) % 100
			);
		printf("\n");
	}

	average_etimea /= stepcount;
	average_etimef /= stepcount;
	average_frag   /= stepcount;
	average_fragi  /= stepcount;
	average_frage  /= stepcount;

	printf("  average,         ,         ,         , %8d,         ,         ,         ,         , %8d,         ,         ,         , %3d.%02d,       ,       ,       , %3d.%02d,       ,       ,       , %3d.%02d,       ,       ,       \n",
			(unsigned int) (average_etimea/1000),
			(unsigned int) (average_etimef/1000),
			(unsigned int) (average_frag  *100), (unsigned int) (average_frag  *10000) % 100,
			(unsigned int) (average_fragi *100), (unsigned int) (average_fragi *10000) % 100,
			(unsigned int) (average_frage *100), (unsigned int) (average_frage *10000) % 100	);

	free(result_p);
	free(heap_ptr);
	free(blist_p);
}

#endif /* !(UBINOS__UBICLIB_TEST__EXCLUDE_HEAPTEST == 1) */
#endif /* (INCLUDE__UBINOS__UBICLIB_TEST == 1) */

