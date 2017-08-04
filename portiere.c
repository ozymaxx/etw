#include "eat.h"
#include "SDL.h"

// recorder
extern unsigned long timest;
extern SDL_Surface *screen;
extern trec *rb;
extern BOOL replay_mode;
extern unsigned long totalTime;

struct pos portieri[2][2][SECTORS+SPECIALS]=
{
    {
        {
        {((FIELD_WIDTH-10)*8)-170*8,252*8},
        {((FIELD_WIDTH-10)*8)-160*8,252*8},
        {((FIELD_WIDTH-10)*8)-144*8,221*8},
        {((FIELD_WIDTH-10)*8)-105*8,202*8},
        {((FIELD_WIDTH-10)*8)-170*8,252*8},
        {((FIELD_WIDTH-10)*8)-150*8,252*8},
        {((FIELD_WIDTH-10)*8)-110*8,252*8},
        {((FIELD_WIDTH-10)*8)-59*8,252*8},
        {((FIELD_WIDTH-10)*8)-170*8,252*8},
        {((FIELD_WIDTH-10)*8)-160*8,252*8},
        {((FIELD_WIDTH-10)*8)-144*8,340*8},
        {((FIELD_WIDTH-10)*8)-105*8,324*8},
        {((FIELD_WIDTH-10)*8)-60*8,CENTROCAMPO_Y+150}, // CORNER_N
        {((FIELD_WIDTH-10)*8)-70*8,CENTROCAMPO_Y-250}, // CORNER_S
        {((FIELD_WIDTH-10)*8)-36*8,252*8}, // PENALTY
        {((FIELD_WIDTH-10)*8)-59*8,252*8}, // GOALKICK
        {((FIELD_WIDTH-10)*8)-59*8,252*8}  // KICKOFF
        },
        {
        {((FIELD_WIDTH-10)*8)-180*8,252*8},
        {((FIELD_WIDTH-10)*8)-170*8,252*8},
        {((FIELD_WIDTH-10)*8)-154*8,221*8},
        {((FIELD_WIDTH-10)*8)-120*8,202*8},
        {((FIELD_WIDTH-10)*8)-180*8,252*8},
        {((FIELD_WIDTH-10)*8)-160*8,252*8},
        {((FIELD_WIDTH-10)*8)-120*8,252*8},
        {((FIELD_WIDTH-10)*8)-69*8,252*8},
        {((FIELD_WIDTH-10)*8)-180*8,252*8},
        {((FIELD_WIDTH-10)*8)-170*8,252*8},
        {((FIELD_WIDTH-10)*8)-154*8,340*8},
        {((FIELD_WIDTH-10)*8)-120*8,324*8},
        {((FIELD_WIDTH-10)*8)-60*8,CENTROCAMPO_Y+150}, // CORNER_N
        {((FIELD_WIDTH-10)*8)-70*8,CENTROCAMPO_Y-250}, // CORNER_S
        {((FIELD_WIDTH-10)*8)-36*8,252*8}, // PENALTY
        {((FIELD_WIDTH-10)*8)-59*8,252*8}, // GOALKICK
        {((FIELD_WIDTH-10)*8)-59*8,252*8}  // KICKOFF
        }
    },
    {
        {
        {60*8,225*8},
        {75*8,227*8},
        {100*8,256*8},
        {150*8,256*8}, 
        {59*8,255*8},    // area rigore
        {90*8,255*8},  // centrocampo avanti
        {100*8,256*8},
        {150*8,256*8},
        {55*8,279*8}, // era 85
        {72*8,272*8},
        {100*8,256*8},
        {150*8,256*8},
        {54*8,265*8}, // CORNER_N
        {57*8,242*8}, // CORNER_S
        {38*8,257*8}, // PENALTY
        {59*8,256*8}, // GOALKICK
        {59*8,256*8}  // KICKOFF
       },
       {

        {60*8,225*8},
        {75*8,227*8},
        {100*8,256*8},
        {150*8,256*8}, 
        {59*8,255*8},    // area rigore
        {90*8,255*8},  // centrocampo avanti
        {100*8,256*8},
        {150*8,256*8},
        {55*8,279*8}, // era 85
        {72*8,272*8},
        {100*8,256*8},
        {150*8,256*8},
        {54*8,265*8}, // CORNER_N
        {57*8,242*8}, // CORNER_S
        {38*8,257*8}, // PENALTY
        {59*8,256*8}, // GOALKICK
        {59*8,256*8}  // KICKOFF
       }
    }
};

void CheckPortiere(int n)
{
    keeper_t *g=&p->team[n]->keepers;
    
    if(g->Special)
        return;

    if(pl->TipoTiro==TIRO_ALTO || pl->TipoTiro==TIRO_PALLONETTO)
    {
        g->SpecialData=-1;

        if(n)
        {
            if(    pl->world_x>AREA_RIGORE_X_O ||
                pl->world_y>AREA_RIGORE_Y_S ||
                pl->world_y<AREA_RIGORE_Y_N )
            {
                g->SpecialData=FindDirection(g->world_x,g->world_y,PORTA_O_X,PORTA_O_Y);
            }
        }
        else
        {
            if(    pl->world_x>AREA_RIGORE_X_E ||
                pl->world_y>AREA_RIGORE_Y_S ||
                pl->world_y<AREA_RIGORE_Y_N )
            {
                g->SpecialData=FindDirection(g->world_x,g->world_y,PORTA_E_X,PORTA_E_Y);
            }
        }

        if(g->SpecialData!=-1)
        {
            g->dir=opposto[g->SpecialData];
            DoAnim(g,PORTIERE_CAMMINA_LATERALE);
            g->ActualSpeed=0;
            g->Tick=10;
        }
    }
}

void HandleGoalKick(keeper_t *g)
{
    if(p->show_panel & 0xff00)
        return;

    if(g->team->Joystick >= 0)
    {
        uint32_t l = r_controls[g->team->Joystick][counter];

        g->Tick--;

        if(g->Tick<0 )
            goto rinviocomputer;
    
        if(l&MYBUTTONMASK)
        {
                g->FirePressed=TRUE;
                g->TimePress++;

                if(g->TimePress>6)
                {
        goto rinvio;
                }
                return;
        }
        else if (g->FirePressed)
rinvio:
        {
            WORD olddir=g->dir;

            g->FirePressed=FALSE;

            DoSpecialAnim(g,PORTIERE_CORSA);

            g->ActualSpeed=1;

            if(g->dir==D_EST)
            {
                if(l&JPF_JOY_UP)
                    g->dir--;
                else if(l&JPF_JOY_DOWN)
                    g->dir++;

            }
            else if(g->dir==D_OVEST)
            {
                if(l&JPF_JOY_UP)
                    g->dir++;
                else if(l&JPF_JOY_DOWN)
                    g->dir--;
            }

            if(g->dir>7)
                g->dir-=8;

            if(g->dir<0)
                g->dir+=8;

            pl->velocita=20;
            pl->quota=0;
                pl->Rimbalzi=0;
            pl->MaxQuota=18;
            pl->TipoTiro=TIRO_PALLONETTO;

            pl->dir=(olddir<<5)+(GetTable()<<1)-5;

            if(g->dir==D_SUD_EST ||
                g->dir==D_NORD_OVEST )
                pl->dir+=(GetTable()+2)<<1;
            else if(g->dir==D_NORD_EST ||
                g->dir==D_SUD_OVEST )
                pl->dir-=(GetTable()+2)<<1;


            if(olddir==D_EST)
            {
                if(l&JPF_JOY_LEFT)
                {
                    pl->velocita-=5;
                }
                else if(l&JPF_JOY_RIGHT)
                {
                    pl->velocita+=5;
                }
            }
            else
            {
                if(l&JPF_JOY_LEFT)
                {
                    pl->velocita+=5;
                }
                else if(l&JPF_JOY_RIGHT)
                {
                    pl->velocita-=5;
                }
            }

            g->SpecialData=pl->velocita;
            FermaPalla();
            g->dir=olddir;
                        g->TimePress=0;
            g->AnimFrame=4;
                        return;
                }
    }
    else
    {
        g->Tick--;

        if(g->Tick<0)
        {

rinviocomputer:
            g->FirePressed=FALSE;
            DoSpecialAnim(g,PORTIERE_CORSA);
            g->ActualSpeed=1;
            pl->quota=0;
            pl->Rimbalzi=0;
            pl->MaxQuota=18;
            pl->TipoTiro=TIRO_PALLONETTO;
            g->SpecialData=15+(GetTable()<<1);
            FermaPalla();
            pl->dir=(g->dir<<5)-10+(GetTable()<<2);
            g->TimePress=0;
            g->AnimFrame=3;
        }
    }
    
    // recorder
    extern trec *rb;
    struct timeval tv;
    extern unsigned long timest;
	extern SDL_Surface *screen;
	extern unsigned long totalTime;
	gettimeofday( &tv, NULL);
	unsigned long long ts = (unsigned long long) tv.tv_sec * 1000 + (unsigned long long) tv.tv_usec / 1000;
    addToBag( g->world_x, g->world_y, g->team->MarkerRed, 0, totalTime, ts, 0, &rb);
    char* ssName = (char *) malloc( 100 * sizeof(char) );
	sprintf( ssName, "%d/%llu.bmp", timest, ts);
	SDL_SaveBMP( screen, ssName);
	free( ssName);
}

void HandleKeeperControlled(keeper_t *g)
{
    if(p->show_panel & 0xff00)
        return;

    if(g->team->Joystick >= 0)
    {
        uint32_t l;

        g->Tick--;

        if(g->Tick<0&&!(p->show_panel&0xff00) )
            goto rinvioliberocomputer;

        l=r_controls[g->team->Joystick][counter];

        if(l&MYBUTTONMASK)
        {
            g->FirePressed=TRUE;
            g->TimePress++;

            if(g->TimePress>10)
            {
                goto rinvio;
            }
        }
        else if (g->FirePressed)
rinvio:
        {
            WORD olddir=g->dir;

            g->FirePressed=FALSE;

            if(g->dir==D_EST)
            {
                if(l&JPF_JOY_UP)
                    g->dir--;
                else if(l&JPF_JOY_DOWN)
                    g->dir++;

                if(g->TimePress<6)
                    g->AnimType=PORTIERE_RINVIO_MANO_PRIMA_EST;
                else
                    g->AnimType=PORTIERE_RINVIO_LIBERO_PRIMA_EST;
            }
            else if(g->dir==D_OVEST)
            {
                if(l&JPF_JOY_UP)
                    g->dir++;
                else if(l&JPF_JOY_DOWN)
                    g->dir--;

                if(g->TimePress<6)
                    g->AnimType=PORTIERE_RINVIO_MANO_PRIMA_OVEST;
                else
                    g->AnimType=PORTIERE_RINVIO_LIBERO_PRIMA_OVEST;
            }

            if(g->TimePress<6)
            {
                            Passaggio((player_t *)g);
                pl->MaxQuota+=3;
            }
            else
            {
                pl->Rimbalzi=0;
                pl->quota=0;
                pl->velocita=20;
                pl->MaxQuota=23;
                pl->TipoTiro=TIRO_PALLONETTO;

                pl->dir=(olddir<<5)+(GetTable()<<1)-5;

                if(g->dir==D_SUD_EST ||
                    g->dir==D_NORD_OVEST )
                    pl->dir+=(GetTable()+2)<<1;
                else if(g->dir==D_NORD_EST ||
                    g->dir==D_SUD_OVEST )
                    pl->dir-=(GetTable()+2)<<1;
                
                if(olddir==D_EST)
                {
                    if(l&JPF_JOY_LEFT)
                    {
                        pl->velocita-=5;
                    }
                    else if(l&JPF_JOY_RIGHT)
                    {
                        pl->velocita+=5;
                    }
                }
                else
                {
                    if(l&JPF_JOY_LEFT)
                    {
                        pl->velocita+=5;
                    }
                    else if(l&JPF_JOY_RIGHT)
                    {
                        pl->velocita-=5;
                    }
                }
            }


            g->SpecialData=pl->velocita;
            FermaPalla();
            g->dir=olddir;
            g->TimePress=0;
        }
    }
    else
    {
// Computerizzato

        g->Tick--;

        if(p->show_panel&0xff00)
            g->Tick=30;

        if(g->Tick<0)
        {
rinvioliberocomputer:
            if(GetTable()>1)
            {
                if(g->dir==D_EST)
                    g->AnimType=PORTIERE_RINVIO_LIBERO_PRIMA_EST;
                else
                    g->AnimType=PORTIERE_RINVIO_LIBERO_PRIMA_OVEST;

                g->SpecialData=15+(GetTable()<<1);
                pl->Rimbalzi=0;
                pl->quota=0;
                pl->MaxQuota=MAX_QUOTA-1-GetTable();
                pl->TipoTiro=TIRO_PALLONETTO;

                pl->dir=(g->dir<<5)-10+(GetTable()<<2);
            }
            else
            {
                if(g->dir==D_EST)
                    g->AnimType=PORTIERE_RINVIO_MANO_PRIMA_EST;
                else
                    g->AnimType=PORTIERE_RINVIO_MANO_PRIMA_OVEST;

                Passaggio((player_t *)g);
                g->SpecialData=pl->velocita;
                pl->MaxQuota+=3;

                if(pl->MaxQuota>=MAX_QUOTA)
                    pl->MaxQuota=MAX_QUOTA-2;
            }

            FermaPalla();
        }
    }
    
    // recorder
    extern trec *rb;
    struct timeval tv;
    extern unsigned long timest;
	extern SDL_Surface *screen;
	gettimeofday( &tv, NULL);
	unsigned long long ts = (unsigned long long) tv.tv_sec * 1000 + (unsigned long long) tv.tv_usec / 1000;
    addToBag( g->world_x, g->world_y, g->team->MarkerRed, 0, totalTime, ts, 0, &rb);
    char* ssName = (char *) malloc( 100 * sizeof(char) );
	sprintf( ssName, "%d/%llu.bmp", timest, ts);
	SDL_SaveBMP( screen, ssName);
	free( ssName);
}

void HandleKeeper(int num)
{
    BOOL near_porta;
    
    // recorder
    extern unsigned long totalTime;

    register keeper_t *g=&p->team[num]->keepers;
    
    // recorder
    if ( !replay_mode) {
		struct timeval tv;
		gettimeofday( &tv, NULL);
		unsigned long long ts = (unsigned long long) tv.tv_sec * 1000 + (unsigned long long) tv.tv_usec / 1000;
		for ( int i = 0; i < 11; i++) {
			if ( i == 0) {
				if ( ( (int) p->team[num]->MarkerRed ) == 6 ) {
					addToBag( g->world_x, g->world_y, p->team[num]->MarkerRed, 0, totalTime, ts, 0, &pb1[i]);
				}
				else {
					addToBag( g->world_x, g->world_y, p->team[num]->MarkerRed, 0, totalTime, ts, 0, &pb2[i]);
				}
			}
			else {
				if ( p->team[num]->players[i-1].Comando != STAI_FERMO) {
					if ( ( (int) p->team[num]->MarkerRed ) == 6 ) {
						addToBag( p->team[num]->players[i-1].world_x, p->team[num]->players[i-1].world_y, p->team[num]->MarkerRed, p->team[num]->players[i-1].GNum + 1, totalTime, ts, 0, &pb1[i]);
					}
					else {
						addToBag( p->team[num]->players[i-1].world_x, p->team[num]->players[i-1].world_y, p->team[num]->MarkerRed, p->team[num]->players[i-1].GNum + 1, totalTime, ts, 0, &pb2[i]);
					}
				}
			}
		}
	}

    g->Tick--;

    if(g->Tick<g->Attenzione&&!g->Special)
    {
        g->Tick=12;

        if( ! (IsVeryNear(g->world_x,g->world_y,portieri[g->SNum][g->team->Possesso][pl->settore].x,portieri[g->SNum][g->team->Possesso][pl->settore].y) ) )
        {
        g->ActualSpeed=1;
        g->dir=FindDirection(g->world_x,g->world_y,portieri[g->SNum][g->team->Possesso][pl->settore].x,portieri[g->SNum][g->team->Possesso][pl->settore].y);

        if(g->AnimType!=PORTIERE_CORSA)
        {
            DoAnim(g,PORTIERE_CORSA);
        }
        }
        else
        {
        g->dir=FindDirection(g->world_x,g->world_y,pl->world_x-48,pl->world_y-100);
        g->ActualSpeed=0;

        if(    g->world_x>(pl->world_x-800) &&
            g->world_x<(pl->world_x+880) &&
            g->world_y>(pl->world_y-600 ) &&
            g->world_y<(pl->world_y+730 ) )
        {
            g->AnimType=PORTIERE_ATTENTO;
            if(g->AnimFrame>=PortAnim[PORTIERE_ATTENTO].Frames)
                g->AnimFrame=0;
        }
        else
        {
            g->AnimType=PORTIERE_FERMO;
            if(g->AnimFrame>=PortAnim[PORTIERE_FERMO].Frames)
                g->AnimFrame=0;
        }
        }
    }

    if(g->ActualSpeed>0)
    {
        g->world_x+=velocita_x[g->ActualSpeed][g->velocita][g->dir];
        g->world_y+=velocita_y[g->ActualSpeed][g->velocita][g->dir];
    }
    else if(g->Special)
    {
        if(g->AnimType>=PORTIERE_TUFFO_DX_ALTO &&
            g->AnimType<=PORTIERE_TUFFO_SX_BASSO
            && g->SpecialData>=0 &&g->SpecialData<256 && g->TimePress)
        {
            g->world_y+=-(cos_table[g->SpecialData]*g->TimePress)>>8;
            g->world_x+=(sin_table[g->SpecialData]*g->TimePress)>>8;
        }
    }
    else if(g->AnimType==PORTIERE_CAMMINA_LATERALE)
    {
        g->world_x+=velocita_x[1][g->velocita][g->SpecialData];
        g->world_y+=velocita_y[1][g->velocita][g->SpecialData];
    }
    
    near_porta=(pl->world_y>(GOAL_Y_N-200)&&pl->world_y<(GOAL_Y_S+150) &&
                ( (g->SNum && pl->world_x<(GOALKICK_X_SO+100)) ||
                  (!g->SNum && pl->world_x>(GOALKICK_X_SE-100)) ) );

    if( (InAreaPortierePalla(g) || near_porta )&&!g->Special&&pl->InGioco)
    {
            if(     pl->world_x>(g->world_x-44)  &&
                    pl->world_x<(g->world_x+170) &&
                    pl->world_y>(g->world_y+75) &&
    /*        g->team!=pl->sq_palla    && */
                    pl->world_y<(g->world_y+200) )
        {
			struct timeval tv;
			gettimeofday( &tv, NULL);
			unsigned long long ts = (unsigned long long) tv.tv_sec * 1000 + (unsigned long long) tv.tv_usec / 1000;
			
            BOOL presa=FALSE;

            if(pl->gioc_palla)
            {
                if((g->Parata+GetTable()+2)>pl->gioc_palla->technique)
                {
                    DoSpecialAnim(pl->gioc_palla,GIOCATORE_CADUTA);

//                    if(pl->gioc_palla->ActualSpeed>0)

                    pl->gioc_palla->ActualSpeed=0;

                    if(!replay_mode)
                        UrgentSpeaker(S_USCITA);

                    TogliPalla();
                    presa=TRUE;
                    
                    if (!replay_mode) {
						addToBag(g->world_x,g->world_y,g->team->MarkerRed,0,totalTime,ts,-6,&rb);
					}
                    
                    DoSpecialAnim(g,PORTIERE_TUFFO_USCITA_PRENDI);
                    g->ActualSpeed=1;
                }
                else
                {
					if (!replay_mode) {
						addToBag(g->world_x,g->world_y,g->team->MarkerRed,0,totalTime,ts,-6,&rb);
					}
					
                    DoSpecialAnim(g,PORTIERE_TUFFO_USCITA_MANCA);
                    g->ActualSpeed=1;
                }
            }
            else
            {
                g->ActualSpeed=0;
                presa=TRUE;

                if(pl->quota<3)
                {
					if (!replay_mode) {
						addToBag(g->world_x,g->world_y,g->team->MarkerRed,0,totalTime,ts,-6,&rb);
					}	
					
                    DoSpecialAnim(g,PORTIERE_RACCOGLI_PALLA);
                }
                else if(pl->quota<6)
                {
                    DoSpecialAnim(g,PORTIERE_PRENDI_MEZZARIA);
                }
                else if(pl->quota<14)
                {
					if (!replay_mode) {
						addToBag(g->world_x,g->world_y,g->team->MarkerRed,0,totalTime,ts,-6,&rb);
					}
					
                    DoSpecialAnim(g,PORTIERE_SALTO_PRENDI);
                }
                else
                {
					if (!replay_mode) {
						addToBag(g->world_x,g->world_y,g->team->MarkerRed,0,totalTime,ts,-6,&rb);
					}
					
                    DoSpecialAnim(g,PORTIERE_SALTO_MANCA);
                    presa=FALSE;
                }
            }

            if(presa)
            {
                FermaPalla();
                pl->InGioco=FALSE;
                pl->settore=GOALKICK;
                g->team->Possesso=1;
                p->team[g->SNum^1]->Possesso=0;
            }
//        Qui devo mettere il codice che gestisce il passaggio all'indietro...

        }
        else if(!pl->gioc_palla && (near_porta || (GetTable()>1&&g->Attenzione>(GetTable())) ) )
        {
            WORD dest_x,dest_y,distance;
            WORD gxl=g->world_x-44,
                gxr=g->world_x+150,
                gyu=g->world_y+75,
                gyd=g->world_y+200,
                newstage=pl->Stage;


            dest_x=G2P_X(pl->world_x);
            dest_y=G2P_Y(pl->world_y);

            for(distance=0;distance<13;distance++)
            {
                dest_x+=pl->delta_x;
                dest_y+=pl->delta_y;
                newstage++;

                    if(     dest_x>gxl &&
                            dest_x<gxr &&
                    dest_y>gyu &&
                    dest_y<gyd )
                    goto skiptuffo;
                else if(newstage>(SHOT_LENGTH-1)&&pl->TipoTiro!=TIRO_RASOTERRA)
                    goto skiptuffo;

            }

// Limiti per non far tuffare il portiere dentro la rete.

            if(g->SNum)
            {
                if(dest_x<(39*8))
                {
                    WORD temp=(39*8)-dest_x;
                    temp/=3;
                    temp*=2;

                    dest_x=(39*8);

                    if(dest_y<g->world_y)
                        dest_y-=temp;
                    else dest_y+=temp;
                }
            }
            else
            {
                if(dest_x>(1236*8))
                {
                    WORD temp=dest_x-(1236*8);

                    temp/=3;
                    temp*=2;

                    dest_x=(1236*8);

                    if(dest_y<g->world_y)
                        dest_y-=temp;
                    else dest_y+=temp;
                }
            }

            g->SpecialData=FindDirection256(g->world_x,g->world_y,dest_x,dest_y-80); // Il -100 per recuperare il problema della palla piu' piccola rispetto al portiere

            distance=FindDistance(g->world_x,g->world_y,dest_x,dest_y,g->SpecialData);

            if(distance>8  && ( near_porta || distance<(g->Parata<<5) )  )
            {
                BYTE temp=CambioDirezione[g->dir][g->SpecialData>>5];

                distance=min((g->Parata<<5),distance);

                g->TimePress=min(distance/6,24+(g->Parata<<3)); //Arrivo max a 50


//                D(bug("Preparo tuffo: dst:%ld dir:%ld\n",distance,g->SpecialData));

                g->ActualSpeed=0;
                
                struct timeval tv;
				gettimeofday( &tv, NULL);
				unsigned long long ts = (unsigned long long) tv.tv_sec * 1000 + (unsigned long long) tv.tv_usec / 1000;

                if(pl->quota<8)
                {
                    if(temp<0)
                    {
						if (!replay_mode) {
							addToBag(g->world_x,g->world_y,g->team->MarkerRed,0,totalTime,ts,-6,&rb);
						}	
						
                        DoSpecialAnim(g,PORTIERE_TUFFO_SX_BASSO);
                        g->dir+=(temp+2);

                    }
                    else if(temp>0&&temp<4)
                    {
						if (!replay_mode) {
							addToBag(g->world_x,g->world_y,g->team->MarkerRed,0,totalTime,ts,-6,&rb);
						}	
						
                        DoSpecialAnim(g,PORTIERE_TUFFO_DX_BASSO);
                        g->dir+=(temp-2);
                    }
                    else
                    {
/*
                        if(temp==4)
                        {
                            g->dir=g->SpecialData>>5;
                            g->ActualSpeed=1;
                            DoSpecialAnim(g,PORTIERE_TUFFO_USCITA_MANCA);
                        }
*/
                        g->SpecialData=-1;
                        g->TimePress=0;
                    }
                }
                else if(pl->quota<18)
                {
                    if(temp<0)
                    {
						if (!replay_mode) {
							addToBag(g->world_x,g->world_y,g->team->MarkerRed,0,totalTime,ts,-6,&rb);
						}	
						
                        DoSpecialAnim(g,PORTIERE_TUFFO_SX_ALTO);
                        g->dir+=(temp+2);
                    }
                    else if(temp>0&&temp<4)
                    {
						if (!replay_mode) {
							addToBag(g->world_x,g->world_y,g->team->MarkerRed,0,totalTime,ts,-6,&rb);
						}	
						
                        DoSpecialAnim(g,PORTIERE_TUFFO_DX_ALTO);
                        g->dir+=(temp-2);
                    }
                    else
                    {
/*
                        if(temp==4)
                        {
                            DoSpecialAnim(g,PORTIERE_SALTO_MANCA);
                        }
*/
                        g->SpecialData=-1;
                        g->TimePress=0;
                    }
                }
                else
                {
                    g->SpecialData=-1;
                    g->TimePress=0;
                }

                if(g->dir<0)
                    g->dir+=8;
                else if(g->dir>7)
                    g->dir-=8;
            }
        }
        else if(( (pl->gioc_palla&&pl->gioc_palla->team!=g->team) || (!pl->gioc_palla && pl->velocita<10) ) && 
            g->world_y<(AREA_RIGORE_Y_S-100)&&g->world_y>(AREA_RIGORE_Y_N+100)&&
            (g->world_x>AREA_RIGORE_X_E ||g->world_x<AREA_RIGORE_X_O ) )
        {
            BOOL presa=FALSE;
// Tuffo sui piedi, da fare...
            if(SimpleDistance(g->world_x,g->world_y,G2P_X(pl->world_x),G2P_Y(pl->world_y))>120)
            {
                g->dir=FindDirection(g->world_x,g->world_y,G2P_X(pl->world_x),G2P_Y(pl->world_y));
                g->ActualSpeed=2;
                g->Tick=g->Attenzione+2;

                if(g->AnimType!=PORTIERE_CORSA)
                {
                    DoAnim(g,PORTIERE_CORSA);
                }
            }
            else if(pl->gioc_palla)
            {
                g->ActualSpeed=1;

                if((g->Parata+GetTable()+3)>pl->gioc_palla->technique)
                {
                    DoSpecialAnim(pl->gioc_palla,GIOCATORE_CADUTA);

                    pl->gioc_palla->ActualSpeed=0;

                    if(!replay_mode)
                        UrgentSpeaker(S_USCITA);

                    TogliPalla();
                    presa=TRUE;
                    DoSpecialAnim(g,PORTIERE_TUFFO_USCITA_PRENDI);
                }
                else
                {
                    DoSpecialAnim(g,PORTIERE_TUFFO_USCITA_MANCA);

                }
            }
            else
            {
                g->ActualSpeed=0;
                presa=TRUE;
                DoSpecialAnim(g,PORTIERE_RACCOGLI_PALLA);
            }

            if(presa)
            {
                FermaPalla();
                pl->InGioco=FALSE;
                pl->settore=GOALKICK;
                g->team->Possesso=1;
                p->team[g->SNum^1]->Possesso=0;
            }
        }
    }

skiptuffo:

    if(g->FrameLen<=0)
    {
        g->AnimFrame++;

        if(g->AnimFrame>=PortAnim[g->AnimType].Frames)
        {
            g->AnimFrame=0;

            if(g->Special)
            {
                switch(g->AnimType)
                {
                    case PORTIERE_PRENDI_MEZZARIA:
                    case PORTIERE_SALTO_PRENDI:
                        p->team[g->SNum^1]->Tiri++;
                    case PORTIERE_TERRA_PALLA:
                    case PORTIERE_TUFFO_USCITA_PRENDI:
                    case PORTIERE_RACCOGLI_PALLA:
                        p->mantieni_distanza=TRUE;
                        g->AnimType=PORTIERE_FERMO_PALLA;

                        if(g->team->Joystick<0)
                            g->Tick=30+(GetTable()<<3);
                        else
                            g->Tick=100;

                        if(g->team==p->team[0])
                            g->dir=D_OVEST;
                        else
                            g->dir=D_EST;

                        break;
                    case PORTIERE_TUFFO_DX_ALTO_PRENDI:
                    case PORTIERE_TUFFO_DX_BASSO_PRENDI:
                        g->dir+=4;

                        if(g->dir>9)
                            g->dir-=8;
                    case PORTIERE_TUFFO_SX_ALTO_PRENDI:
                    case PORTIERE_TUFFO_SX_BASSO_PRENDI:
                        g->dir-=2;
                        if(g->dir<0)
                            g->dir+=8;

                        g->AnimType=PORTIERE_TERRA_PALLA;
                        break;
                    case PORTIERE_TUFFO_DX_ALTO_MANCA:
                    case PORTIERE_TUFFO_DX_BASSO_MANCA:
                        g->dir+=4;

                        if(g->dir>9)
                            g->dir-=8;
                    case PORTIERE_TUFFO_SX_ALTO_MANCA:
                    case PORTIERE_TUFFO_SX_BASSO_MANCA:
                        g->dir-=2;
                        if(g->dir<0)
                            g->dir+=8;

                        g->AnimType=PORTIERE_TERRA;
                        break;
                    case PORTIERE_TUFFO_DX_ALTO:
                    case PORTIERE_TUFFO_DX_BASSO:
                    case PORTIERE_TUFFO_SX_ALTO:
                    case PORTIERE_TUFFO_SX_BASSO:
                        if(NeiPressiPallaPortiere(g)&&pl->quota<14)
                        {
                            p->team[g->SNum^1]->Tiri++;

                            if(pl->velocita<((g->Parata<<2)-(GetTable()<<1)+2)&&pl->InGioco)
                            {
                                g->SpecialData=-1;
                                g->TimePress=0;

                                if(!replay_mode)
                                {
                                    UrgentSpeaker(S_PARATA);
                                }

                                p->team[g->SNum]->Possesso=1;
                                p->team[g->SNum^1]->Possesso=0;
                                pl->InGioco=FALSE;
                                FermaPalla();
                                HideBall();

                                if (pl->quota<6)
                                {
                                    if(g->AnimType==PORTIERE_TUFFO_SX_ALTO||g->AnimType==PORTIERE_TUFFO_SX_BASSO)
                                    {
                                        g->AnimType=PORTIERE_TUFFO_SX_BASSO_PRENDI;
                                    }
                                    else
                                        g->AnimType=PORTIERE_TUFFO_DX_BASSO_PRENDI;
                                }
                                else
                                {
                                    if(g->AnimType==PORTIERE_TUFFO_SX_ALTO||g->AnimType==PORTIERE_TUFFO_SX_BASSO)
                                    {
                                        g->AnimType=PORTIERE_TUFFO_SX_ALTO_PRENDI;
                                    }
                                    else
                                        g->AnimType=PORTIERE_TUFFO_DX_ALTO_PRENDI;
                                }
                                break;
                            }
                            else if(pl->quota<13&&pl->InGioco)
                            {
                                if(!replay_mode)
                                {
                                    UrgentSpeaker(S_PARATA_NON_BLOCCA);
                                }

                                if(pl->Stage>(SHOT_LENGTH/2))
                                {
                                    pl->Stage=0;

                                    while(p->shotheight[pl->Stage]<pl->quota)
                                        pl->Stage++;
                                }

                                pl->velocita-=(GetTable()+3);

                                if(pl->velocita>12)
                                    pl->velocita-=(GetTable()+2);

                                if(pl->velocita<=0)
                                    pl->velocita=2;

                                p->team[g->SNum]->Possesso=1;
                                p->team[g->SNum^1]->Possesso=0;
                                
                                pl->dir=(g->SpecialData-(GetTable()<<2)+10);
                                
                                UpdateBallSpeed();

                                PlayBackSound(sound[CONTRASTO]);
                            }
                        }

                        g->SpecialData=-1;
                        g->TimePress=0;

                        if(pl->quota>6)
                        {
                            if(g->AnimType==PORTIERE_TUFFO_SX_ALTO||g->AnimType==PORTIERE_TUFFO_SX_BASSO)
                            {
                                g->AnimType=PORTIERE_TUFFO_SX_ALTO_MANCA;
                            }
                            else
                                g->AnimType=PORTIERE_TUFFO_DX_ALTO_MANCA;
                        }
                        else
                        {
                            if(g->AnimType==PORTIERE_TUFFO_SX_ALTO||g->AnimType==PORTIERE_TUFFO_SX_BASSO)
                            {
                                g->AnimType=PORTIERE_TUFFO_SX_BASSO_MANCA;
                            }
                            else
                                g->AnimType=PORTIERE_TUFFO_DX_BASSO_MANCA;
                        }
    
                        break;
                    case PORTIERE_RINVIO_LIBERO_PRIMA_EST:
                        p->mantieni_distanza=FALSE;
                        pl->quota=2;
                        pl->Stage=4;
                        pl->sq_palla=NULL;
                        pl->world_x=g->world_x+100;
                        pl->world_y=g->world_y+100;
                        pl->velocita=g->SpecialData;
                        UpdateBallSpeed();
                        UpdateShotHeight();
                        pl->InGioco=TRUE;
                        RestartTime();
                        ShowBall();
                        PlayBackSound(sound[SHOOT]);
                        g->AnimType=PORTIERE_RINVIO_PIEDE_EST;
                        break;
                    case PORTIERE_RINVIO_LIBERO_PRIMA_OVEST:
                        p->mantieni_distanza=FALSE;
                        pl->quota=2;
                        pl->Stage=4;
                        pl->sq_palla=NULL;
                        pl->world_x=g->world_x-80;
                        pl->world_y=g->world_y+100;
                        pl->velocita=g->SpecialData;
                        UpdateBallSpeed();
                        UpdateShotHeight();
                        pl->InGioco=TRUE;
                        RestartTime();
                        ShowBall();
                        PlayBackSound(sound[SHOOT]);
                        g->AnimType=PORTIERE_RINVIO_PIEDE_OVEST;
                        break;
                    case PORTIERE_FERMO:
                        g->ActualSpeed=0;
                        HandleGoalKick(g);
                        break;
                    case PORTIERE_CORSA:
                        if(g->dir==D_OVEST)
                            g->AnimType=PORTIERE_RINVIO_PIEDE_OVEST;
                        else
                            g->AnimType=PORTIERE_RINVIO_PIEDE_EST;

                        g->ActualSpeed=0;
                        pl->InGioco=TRUE;
                        pl->sq_palla=NULL;
                        pl->velocita=g->SpecialData;
                        UpdateBallSpeed();
                        UpdateShotHeight();
                        RestartTime();
                        PlayBackSound(sound[SHOOT]);
                        break;
                    case PORTIERE_FERMO_PALLA:
                        g->ActualSpeed=0;

                        if(!penalties&&!free_kicks)
                        {
                            player_t *g2=p->team[g->SNum^1]->attivo;

                            if(g2 && InArea(g->SNum,P2G_X(g2->world_x),P2G_Y(g2->world_y)))
                                g2->Comando=ESCI_AREA;

                            HandleKeeperControlled(g);
                        }
                        else
                        {
                            pl->quota=0;
                            pl->world_x=g->world_x+100;
                            pl->world_y=g->world_y+100;
                            pl->world_x=RIGORE_X_O;    
                            pl->world_y=RIGORE_Y;
                            pl->settore=PENALTY;
                            pl->velocita=0;
                            g->Special=FALSE;
                            g->ActualSpeed=0;
                            g->AnimType=PORTIERE_FERMO;
                            TogliPalla();
                            ShowBall();
                        }
                        break;
                    case PORTIERE_RINVIO_MANO_PRIMA_EST:
                        p->mantieni_distanza=FALSE;
                        pl->quota=3;
                        pl->Stage=5;
                        pl->world_x=g->world_x+100;
                        pl->world_y=g->world_y+100;
                        ShowBall();
                        pl->sq_palla=NULL;
                        pl->InGioco=TRUE;
                        RestartTime();
                        pl->velocita=g->SpecialData;
                        UpdateBallSpeed();
                        UpdateShotHeight();
                        g->AnimType=PORTIERE_RINVIO_MANO_DOPO_EST;
                        break;        
                    case PORTIERE_RINVIO_MANO_PRIMA_OVEST:
                        p->mantieni_distanza=FALSE;
                        pl->quota=3;
                        pl->Stage=5;
                        pl->world_x=g->world_x-80;
                        pl->world_y=g->world_y+100;
                        pl->velocita=g->SpecialData;
                        pl->sq_palla=NULL;
                        UpdateBallSpeed();
                        UpdateShotHeight();
                        pl->InGioco=TRUE;
                        RestartTime();
                        ShowBall();
                        g->AnimType=PORTIERE_RINVIO_MANO_DOPO_OVEST;
                        break;
                    case PORTIERE_PRERIGORE:
                        if(GetTable()>2)
                            g->SpecialData= g->SNum ? 133 : 123;
                        else
                            g->SpecialData= g->SNum ? 5 : 251;

                        {
                            BYTE temp=CambioDirezione[g->dir][g->SpecialData>>5];

                            if(temp<0)
                            {
                                g->AnimType=PORTIERE_TUFFO_SX_BASSO;
                                g->dir+=(temp+2);
                            }
                            else
                            {
                                g->AnimType=PORTIERE_TUFFO_DX_BASSO;
                                g->dir+=(temp-2);
                            }
                        }

                        g->TimePress=24;
                        g->ActualSpeed=0;
                        break;
                    default:
                        g->AnimType=PORTIERE_FERMO;
                        g->ActualSpeed=0;
                        g->Special=FALSE;
                }
            }
        }
        else if(g->Special)
        {
            BOOL presa=FALSE;

            switch(g->AnimType)
            {
                case PORTIERE_PRENDI_MEZZARIA:
                case PORTIERE_RACCOGLI_PALLA:
                    if(g->AnimFrame==1)
                        presa=TRUE;
                    break;
                case PORTIERE_SALTO_PRENDI:
                    if(g->AnimFrame==4)
                        presa=TRUE;
                    break;
                case PORTIERE_TUFFO_USCITA_PRENDI:
                    if(g->AnimFrame==2)
                    {
                        g->ActualSpeed=0;
                        presa=TRUE;
                    }
                    break;
            }

            if(presa)
            {
                p->team[g->SNum]->Possesso=1;
                p->team[g->SNum^1]->Possesso=0;
                pl->InGioco=FALSE;
                FermaPalla();
                HideBall();
            }

        }

        g->FrameLen=PortAnim[g->AnimType].FrameLen;

        if(g->OnScreen)
            ChangeAnimObj(g->anim,PortAnim[g->AnimType].Frame[g->AnimFrame]+g->dir);
    }

    g->FrameLen--;

}

