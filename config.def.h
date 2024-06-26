/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "?", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *lockcmd[]           = { "slock", NULL };
static const char *shutdown[]          = { "shutdown", "now", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

static const Key keys[] = {
	/*modifier         , key                      , function       , argument*/

	// window management
	{ MODKEY|ShiftMask , XK_q                     , killclient     , {0} },
	{ MODKEY|ShiftMask , XK_e                     , quit           , {0} },
	TAGKEYS(XK_1       , 0)
	TAGKEYS(XK_2       , 1)
	TAGKEYS(XK_3       , 2)
	TAGKEYS(XK_4       , 3)
	TAGKEYS(XK_5       , 4)
	TAGKEYS(XK_6       , 5)
	TAGKEYS(XK_7       , 6)
	TAGKEYS(XK_8       , 7)
	TAGKEYS(XK_9       , 8)
	{ MODKEY           , XK_period                , focusmon         , {.i = +1} }, // move focus one monitor up
	{ MODKEY           , XK_comma                 , focusmon         , {.i = -1} }, // move focus one monitor down
	{ MODKEY           , XK_j                     , focusstack       , {.i = +1} }, // move focus one window up in stack
	{ MODKEY           , XK_k                     , focusstack       , {.i = -1} }, // move focus one window down in stack
	{ MODKEY           , XK_plus                  , incnmaster       , {.i = +1} }, // add one window from number of master windows
	{ MODKEY           , XK_minus                 , incnmaster       , {.i = -1} }, // remove one window from number of master windows
	{ MODKEY|ALTKEY    , XK_minus                 , setgaps          , {.i = -1 } },
	{ MODKEY|ALTKEY    , XK_plus                  , setgaps          , {.i = +1 } },
	{ MODKEY|ALTKEY    , XK_0                     , setgaps          , {.i = 0 } },
	{ MODKEY           , XK_space                 , setlayout        , {0} }, // switch to last layout
	{ MODKEY           , XK_t                     , setlayout        , {.v = &layouts[0]} }, // switch to "tile" layout
	{ MODKEY           , XK_f                     , setlayout        , {.v = &layouts[1]} }, // switch to floating
	{ MODKEY           , XK_o                     , setlayout        , {.v = &layouts[2]} }, // switch to "monocle" layout
	{ MODKEY           , XK_u                     , setlayout        , {.v = &layouts[3]} }, // switch to "centeredmaster" layout
	{ MODKEY           , XK_l                     , setmfact         , {.f = +0.025} }, // increase percentage of master window on monitor
	{ MODKEY           , XK_h                     , setmfact         , {.f = -0.025} }, // decrease percentage of master window on monitor
	{ MODKEY|ShiftMask , XK_period                , tagmon           , {.i = +1} }, // move window tag one monitor up
	{ MODKEY|ShiftMask , XK_comma                 , tagmon           , {.i = -1} }, // move window tag one monitor down
	{ MODKEY|ShiftMask , XK_0                     , tag              , {.ui = ~0} }, // tag window in all tags
	{ MODKEY|ShiftMask , XK_b                     , togglebar        , {0} },
	{ MODKEY|ShiftMask , XK_space                 , togglefloating   , {0} },
	{ MODKEY|ShiftMask , XK_f                     , togglefullscreen , {0} },
	{ MODKEY           , XK_Tab                   , view             , {0} }, // move to last tag
	{ MODKEY           , XK_0                     , view             , {.ui = ~0} }, // view all tags
	{ MODKEY|ShiftMask , XK_Return                , zoom             , {0} }, // change stack master

	// utility shortcuts
	{ MODKEY           , XK_Return                , spawn          , {.v = termcmd} },
	{ 0                , XK_Menu                  , spawn          , {.v = termcmd} },
	{ MODKEY|ShiftMask , XK_F1                    , spawn          , {.v = shutdown} },
	{ MODKEY           , XK_Escape                , spawn          , {.v = lockcmd} },
	{ MODKEY           , XK_F12                   , togglescratch  , {.v = scratchpadcmd } },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

