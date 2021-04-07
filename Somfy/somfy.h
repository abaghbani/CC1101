
enum somfy_ctrl
{
	stop = 0x1,	//	0x1 My	Stop or move to favourite position
	move_up,	//	0x2 Move up
	my_up,		//	0x3 My + Up	Set upper motor limit in initial programming mode
	move_down,	//	0x4	Down	Move down
	my_down,	//	0x5	My + Down	Set lower motor limit in initial programming mode
	up_down,	//	0x6	Up + Down	 Change motor limit and initial programming mode
	prog,		//	0x8	Prog	Used for (de-)registering remotes, see below
	sun_flag,	//	0x9	Sun + Flag	Enable sun and wind detector (SUN and FLAG symbol on the Telis Soliris RC)
	flag,		//	0xA	Flag	Disable sun detector (FLAG symbol on the Telis Soliris RC)
};

void SendPowerOn(CC1101_t *cc1101);
void SendSomfyFrame(CC1101_t *cc1101, uint8_t preamble_num, uint8_t *buf);
void SendSomfyFrame_v2(CC1101_t *cc1101, uint8_t *buf);
void make_somfy_frame(uint8_t *buf, uint8_t ctrl, uint16_t rolling_code, uint32_t address);
