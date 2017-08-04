// this is a fix for OSX
#define MoveTo MyMoveTo

// recorder
extern trec *rb;
extern char* fileName;
extern unsigned long timest;
extern trec **pb1;
extern trec **pb2;
extern unsigned long totalTime;

extern struct player_disk Riserve[2][12];
extern UBYTE goal_array[GA_SIZE],goal_minute[GA_SIZE],goal_team[GA_SIZE],team_a,team_b,TotaleRiserve[2],NumeroTattiche;
extern char team_name[2][16],fieldname[24],palette[24],shirt[2][24];
extern object_t *pezzi_porte[4],*bonus[MAX_ARCADE_ON_FIELD];
extern int8_t need_release[MAX_PLAYERS], arcade_team[2], starting_team;
extern int8_t Table[],strictness,current_field,slowdown[];
extern struct RastPort Sq1RP,Sq2RP,PallaRP,*ProgressRP,ArcadeRP;
extern BOOL quit_game,no_sound,soft_scroll,pause_mode,nosync,final,full_replay,killer,
    start_replay,replay_mode,use_replay,allow_replay,nocpu,teams_swapped,first_kickoff,
    arcade,situation,training,use_crowd,slow_motion,use_speaker,replay_looped,highlight,
    substitutions,bookings,injuries,window_opened,screen_opened,nopari,free_longpass,
    no_record,penalties,free_kicks,arcade_teams,first_half,extratime,left_sel,right_sel,
    friendly,golden_gol,use_key0,use_key1,joyonly,audio2fast,use_offside,
    newpitches,network_game, game_start;
extern char control[4],*controls[CONTROLS],tipo_porta, spk_basename[64];
extern int32_t Pens[256];
extern int framerate, game_status, urgent_status, basepri;
extern long int time_length, t_l, situation_time;
extern WORD velocita_x[3][10][8],cambio_x[8],inversione_x[8][8],arcade_frame_sequence[];
extern WORD velocita_y[3][10][8],cambio_y[8],inversione_y[8][8];
extern int8_t prontezza[10],opposto[],scroll_type,situation_result[2];
extern WORD avanzamento_x[],avanzamento_y[],quota_mod_x[],quota_mod_y[];
extern WORD sin_table[256],cos_table[256],swaps;
extern UWORD counter;
extern WORD av_palla_x[8][12],av_palla_y[8][12],velocita_scivolata_x[],velocita_scivolata_y[];
extern game_t *p;
extern ball_t *pl;
extern struct Animazione Animation[],ArbAnim[],PortAnim[],GLAnim[];
extern WORD field_x,field_y,porte_x[],field_x_limit,field_y_limit;
extern int8_t joy_opposto[],dir_pred[],dir_next[],CambioDirezione[8][8],player_type[4],role[4];
extern int WINDOW_WIDTH, WINDOW_HEIGHT, wanted_sound;
extern struct SoundInfo *sound[];
extern char *soundname[],*tactics[];
extern object_t *object_list[];
extern struct DOggetto *c_list[];
extern int totale_lista,totale_lista_c;
extern struct SignalSemaphore listsem;
extern uint32_t detail_level, *r_controls[MAX_PLAYERS], ahimode;
extern struct Rect ingombri[];
extern struct DOggetto peoples[];
extern gfx_t *last_obj,*pause_gfx;
extern mytimer StartGameTime,EndTime,ideal;
extern anim_t *ports,*replay,*arcade_anim,*goal_banner;
extern linesman_t *linesman;
extern struct MyFastScaleArgs *scaling;
extern int font_width,font_height,result_width,FIXED_SCALING_WIDTH,FIXED_SCALING_HEIGHT;

extern void MakeRef(uint8_t *,int, int);
extern tactic_t *LoadTactic(char *);
extern void FreeTactic(tactic_t *);
extern void InvertTactic(tactic_t *);
extern game_t *SetupSquadre(void);
extern void LiberaPartita(game_t *);
extern WORD FindDirection(WORD,WORD,WORD,WORD);
extern uint32_t ReadKeyPort(uint32_t port);
extern uint32_t ReadNetworkPort(uint32_t port);
extern void ChangeControlled(team_t *, WORD);
extern void HandleControlled(int);
extern void HandleControlledJ2B(int);
extern void HandleControlledJoyPad(int);
extern void HandleCPU(int);
extern void HandleRealCPU(player_t *);
extern void HandleReferee(void);
extern void HandleGuardalinee(void);
extern void HandleExtras(void);
extern void PostHandleBall(void);
extern void HandleBall(void);
extern void HandleKeeper(int);
extern void HandleRimessa(player_t *);
extern void DoSpecials(player_t *);
extern void CheckKeys(void);
extern void CheckActive(void);

// main

extern void SetResult(char *, ...);

// squadre

extern void ReadSquadra(FILE *, struct team_disk *);
extern void DisponiSquadra(team_t *, int, BOOL );
extern void DisponiPortiere(team_t *, int, BOOL );
extern void MakeResult(void);
extern FILE *OpenTeam(char *);
extern void SwapTeams(void);
extern void ChangePlayer(struct player_disk *, player_t *);
extern BOOL NumeroDiverso(team_t *, char);
extern void GL_Fuori(int);

// control

extern void MoveNonControlled(void);
extern WORD GetJoyDirection(uint32_t);
extern void NoPlayerControl(player_t *);
extern void CheckChange(player_t *);
extern uint32_t (*MyReadPort0)(uint32_t);
extern uint32_t (*MyReadPort1)(uint32_t);
extern void UpdatePortStatus(void);

// Special

extern void EseguiEsultanza(int);
extern void CPUShot(player_t *);
extern void EseguiCross(player_t *);
extern void (*LongPass)(player_t *, uint32_t);
extern void FreePass(player_t *, uint32_t);
extern void TargetedPass(player_t *, uint32_t);
extern void Tira(player_t *);
extern void Passaggio(player_t *);
extern void Passaggio2(player_t *,char);
extern void PassaggioB(player_t *);
extern void RimessaLaterale(player_t *);
extern void PunizioneCorner(player_t *);
extern void UpdateCornerLine(void);
extern void ColpoDiTesta(player_t *);
extern void PreparaBarriera(char);
extern BOOL IsOffside(player_t *);

// Utils

// Aggiungo e rimuovo oggetti alla lista degli oggetti presente sul campo

extern player_t *FindNearest(team_t *s,WORD,WORD);
extern void MoveTo(player_t *,WORD,WORD);
extern WORD FindDirection32(WORD, WORD, WORD ,WORD );
extern WORD CanScore(player_t *);
extern player_t *FindNearestPlayer(player_t *);
extern player_t *FindNearestDirPlayer(player_t *);

extern void LiberaListe(void);
extern BOOL AggiungiLista(object_t *);
extern void RimuoviLista(object_t *);
extern BOOL AggiungiCLista(struct DOggetto *);
extern void RimuoviCLista(struct DOggetto *);
extern void VuotaCLista(void);


extern void LoadKeyDef(int, char *);
extern void SaveKeyDef(int, char *);
extern void DoPause(void);
// extern char GetTable(void); e' una ,macro ora
extern void DoFlash(void);
extern void UpdateBallSpeed(void);
extern void UpdateShotHeight(void);
extern UBYTE FindDirection256(WORD, WORD, WORD ,WORD );
extern BOOL AllowRotation(player_t *,WORD);
extern void RimuoviComandoSquadra(char , BYTE );
extern void EseguiDopoComando(player_t *);
extern void SetComando(player_t *, BYTE, BYTE, BYTE);
extern BOOL InArea(BYTE, WORD, WORD);
extern BOOL InAnyArea(WORD, WORD);
extern WORD IndirizzaTiro(player_t *, uint32_t);
extern void SetShotSpeed(player_t *,WORD);

extern void init_crowd(void);
extern void free_crowd(void);
extern BOOL crowd2memory(void);
extern BOOL speaker2memory(void);
extern void init_speaker(void);
extern void free_speaker(void);
extern struct SoundInfo *handle_crowd(void);
extern struct SoundInfo *handle_speaker(void);

// Rimbalzi

extern void RimbalzoCasuale(void);

// Fonts

extern void PrintNames(void);
extern void PrintSmall(uint8_t *, char *, char );
extern void PrintVerySmall(uint8_t *, char *, char,int );
extern void LoadLogo(char *);
extern void LoadPLogo(char *);
extern BOOL InitFonts(void);
extern void FreeFonts(void);
extern void ShowPanel(void);
extern void PrintBottom(char *);
extern void DrawR(void);
extern void DrawPause(void);
extern void ShowFinal(void);

// Suoni

extern void PlayIfNotPlaying(int);
extern BOOL CaricaSuoni(void);
extern void LiberaSuoni(void);
extern void os_start_audio(void);
extern void os_stop_audio(void);
// Radar

extern BOOL big;
extern int radar_position;

extern void (*HandleRadar)(void);
extern void HandleRadarBig(void);
extern void HandleRadarLittle(void);
extern void ResizeRadar(void);

// Loops

extern void MainLoop(void);
extern void OldMainLoop(void);
extern void (*HandleTeam0)(int);
extern void (*HandleTeam1)(int);
extern void (*HandleRadar)(void);
extern gfx_t *background;
extern WORD n_limit,o_limit,s_limit,e_limit;

// Config

extern void read_config(void);
extern void OpenTheScreen(void);

// Replay

extern void HandleReplay(void);
extern BOOL AllocReplayBuffers(void);
extern void FreeReplayBuffers(void);
extern void SaveReplay(void);
extern void CheckPortiere(int);
extern void StopTime(void);
extern void RestartTime(void);
extern void LoadHighlight(void);
extern void RestartGame(void);


#define SPrintf sprintf    

// Arcade

void RemoveArcadeEffect(player_t *,UBYTE);
void GetArcadeEffect(player_t *,object_t *);
void HandleArcade(void);

// Da os_video.c

void AdjustSDLPalette(void);
long obtain_pen(char,char,char);
void lock_pen(int);
void release_pen(long);
void os_set_color(int,int,int,int);
void close_graphics(void);
BOOL window_open(void);
void os_resize(void);
void os_set_window_frame(void);
void os_wait(void);
int os_wait_end_pause(void);
int os_get_screen_width(void);
int os_get_screen_height(void);
BOOL os_create_dbuffer(void);
int os_get_inner_width(void);
int os_get_inner_height(void);
void os_load_palette(uint32_t *);
void os_free_dbuffer(void);
BOOL os_lock_bitmap(void);
void os_unlock_bitmap(void);

// Da os_control.c

void set_controls(void);
void free_joyports(void);
void os_getevent(void);

// Da os_init.c

void init_system(void);
void os_delay(int);
int os_avail_mem(void);
void SetCrowd(int);
void os_init_timer(void);
void os_free_timer(void);
void os_audio2fast(void);

#ifdef CD_VERSION
void UrgentSpeaker(int);
void StartSpoken(void);
#endif

#ifdef DEMOVERSION
void WaitOrKey(int);
#endif

