#include <eadk.h>
#include <stdint.h>
#include <stdio.h>

extern "C" {
const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Cat Simulator";
const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;
}

static eadk_color_t rgb(int r,int g,int b){ return (eadk_color_t)(((r>>3)<<11)|((g>>2)<<5)|(b>>3)); }
static const eadk_color_t BG=rgb(19,29,45), PANEL=rgb(31,48,67), WHITE=rgb(238,244,250), CYAN=rgb(66,190,220), GOLD=rgb(246,192,76), GREEN=rgb(70,185,125), RED=rgb(232,100,100), GRASS=rgb(54,103,76), ROAD=rgb(70,78,90), HOUSE=rgb(154,115,93), WATER=rgb(43,91,145);
static void rect(int x,int y,int w,int h,eadk_color_t c){ if(x<0){w+=x;x=0;} if(y<0){h+=y;y=0;} if(x+w>320)w=320-x;if(y+h>240)h=240-y;if(w>0&&h>0)eadk_display_push_rect_uniform({(uint16_t)x,(uint16_t)y,(uint16_t)w,(uint16_t)h},c); }
static void text(const char*s,int x,int y,bool large=false,eadk_color_t c=WHITE,eadk_color_t bg=BG){eadk_display_draw_string(s,{(uint16_t)x,(uint16_t)y},large,c,bg);}
static void frame(){rect(0,0,320,240,BG);rect(0,0,320,24,PANEL);text("MICROSOFT CAT SIMULATOR",7,4,true,CYAN,PANEL);rect(0,232,320,8,PANEL);}
static int px=4,py=5,energy=92,hunger=24,fun=68,comfort=80,friendship=12,mission=0,zone=0,menu=0,photo=0,weather=0,day=1;
static uint32_t lastTick=0; static bool keyWas[64]={false};
static bool just(eadk_keyboard_state_t k,eadk_key_t key){bool now=eadk_keyboard_key_down(k,key);bool edge=now&&!keyWas[(int)key];keyWas[(int)key]=now;return edge;}
static void drawCat(int x,int y){int xx=35+x*24, yy=43+y*19; rect(xx-6,yy-5,13,12,GOLD); rect(xx-5,yy-9,4,5,GOLD);rect(xx+1,yy-9,4,5,GOLD);rect(xx-3,yy-1,2,2,BG);rect(xx+2,yy-1,2,2,BG);rect(xx,yy+3,2,1,RED);}
static void mapDraw(){frame();text("NEIGHBORHOOD",8,29,false,GOLD,BG); // mini map
 for(int y=0;y<9;y++)for(int x=0;x<11;x++){eadk_color_t c=GRASS; if(y==3||y==7)c=ROAD; if(x==5)c=ROAD; if(x==9&&y<7)c=WATER; if((x==1&&y==1)||(x==7&&y==1)||(x==3&&y==5)||(x==8&&y==5))c=HOUSE; rect(34+x*24,42+y*19,22,17,c); }
 drawCat(px,py); text("E explore   OK interact",8,215,false,CYAN,BG);text("BACK menu",8,225,false,WHITE,BG); text("DAY",270,29,false,GOLD,BG); char d[10];sprintf(d,"%d",day);text(d,290,29,false,WHITE,BG);
}
static void status(){frame();text("CAT STATUS",8,32,true,GOLD,BG); char b[40];
 sprintf(b,"Energy   %d%%",energy);text(b,12,61,false,WHITE,BG);rect(12,74,140,8,PANEL);rect(12,74,energy*140/100,8,GREEN);
 sprintf(b,"Hunger   %d%%",hunger);text(b,12,91,false,WHITE,BG);rect(12,104,140,8,PANEL);rect(12,104,hunger*140/100,8,RED);
 sprintf(b,"Fun      %d%%",fun);text(b,12,121,false,WHITE,BG);rect(12,134,140,8,PANEL);rect(12,134,fun*140/100,8,CYAN);
 sprintf(b,"Comfort   %d%%",comfort);text(b,12,151,false,WHITE,BG);rect(12,164,140,8,PANEL);rect(12,164,comfort*140/100,8,GOLD);
 sprintf(b,"Bond      %d",friendship);text(b,12,184,false,WHITE,BG);text("OK: back to world",8,216,false,CYAN,BG);
 text("MOOD",190,60,true,WHITE,BG);text(energy<30?"Sleepy":hunger>70?"Hungry":fun<25?"Bored":"Curious",190,85,false,GOLD,BG);text("Weather",190,119,false,WHITE,BG);text(weather==0?"Sunny":weather==1?"Cloudy":"Rain",190,136,false,CYAN,BG);text("Territory",190,168,false,WHITE,BG);text(zone==0?"Garden":"Town",190,185,false,GREEN,BG);
}
static void pauseMenu(){frame();text("PAUSE / CAT MENU",10,35,true,GOLD,BG);text("UP/DOWN select, OK",10,58,false,WHITE,BG); const char*opts[]={"Resume","Cat status","Take a nap","Play with ball","Explore town","Photo mode","Weather","Quit to title"};for(int i=0;i<8;i++){rect(12,76+i*17,296,15,i==menu?CYAN:PANEL);text(opts[i],18,78+i*17,false,i==menu?BG:WHITE,i==menu?CYAN:PANEL);} }
static void title(){frame();text("CAT SIMULATOR",20,45,true,GOLD,BG);text("ALPHA 0.1 - NWA EDITION",20,68,false,CYAN,BG);rect(18,95,284,90,GRASS); // stylized cat sprite
 rect(139,127,44,30,GOLD);rect(145,119,10,12,GOLD);rect(170,119,10,12,GOLD);rect(149,136,5,4,BG);rect(168,136,5,4,BG);rect(158,144,7,3,RED);rect(128,151,22,7,GOLD);rect(179,151,22,7,GOLD);text("A tiny world. A big cat life.",42,169,false,WHITE,GRASS);text("Press OK to start",76,205,true,CYAN,BG);text("Arrows move | OK interact | BACK menu",12,225,false,WHITE,BG);}
static void message(const char *a,const char*b){frame();text("INTERACTION",10,35,true,GOLD,BG);text(a,10,80,false,WHITE,BG);text(b,10,105,false,CYAN,BG);text("OK to continue",10,210,false,WHITE,BG);}
static int screen=0, interact=0;
int main(){title(); while(true){eadk_keyboard_state_t k=eadk_keyboard_scan(); bool ok=just(k,eadk_key_ok)||just(k,eadk_key_exe); bool back=just(k,eadk_key_back)||just(k,eadk_key_backspace); bool up=just(k,eadk_key_up),dn=just(k,eadk_key_down),left=just(k,eadk_key_left),right=just(k,eadk_key_right); uint32_t now=eadk_timing_millis();
 if(screen==0){if(ok){screen=1;mapDraw();} }
 else if(screen==1){if(back){screen=2;menu=0;pauseMenu();}else if(up&&py>0)py--;else if(dn&&py<8)py++;else if(left&&px>0)px--;else if(right&&px<10)px++; if(ok){interact=(px+py)%5;screen=3;message(interact==0?"A sunlit patch of grass.":interact==1?"A ball rolls nearby.":interact==2?"A friendly human waves.":interact==3?"A cardboard box!":"You discovered a new path.","Your cat enjoys the moment.");} if(now-lastTick>3500){lastTick=now;hunger=hunger<96?hunger+1:100;fun=fun>0?fun-1:0;energy=energy>0?energy-1:0;day++;} if(screen==1)mapDraw();}
 else if(screen==2){if(back){screen=1;mapDraw();}else{if(up)menu=(menu+7)%8;if(dn)menu=(menu+1)%8;if(ok){if(menu==0){screen=1;mapDraw();}else if(menu==1){screen=4;status();}else if(menu==2){energy=energy+25>100?100:energy+25;screen=3;message("Nap time!","Energy restored.");}else if(menu==3){fun=fun+22>100?100:fun+22;screen=3;message("You played with a ball!","Fun increased.");}else if(menu==4){zone=1;px=5;py=4;screen=1;mapDraw();}else if(menu==5){photo=1;screen=3;message("Photo mode","Press OK for pose, BACK to exit.");}else if(menu==6){weather=(weather+1)%3;screen=1;mapDraw();}else{screen=0;title();}}pauseMenu();}}
 else if(screen==3){if(ok){if(interact==1){fun=fun+15>100?100:fun+15;}if(interact==2){friendship++;comfort=comfort+5>100?100:comfort+5;}if(interact==0){comfort=comfort+5>100?100:comfort+5;}if(interact==4)mission++;screen=1;mapDraw();}else if(back){screen=1;mapDraw();}}
 else if(screen==4){if(ok||back){screen=1;mapDraw();}}
 eadk_timing_msleep(80);
 }
}
