/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

#define SS_DIR          TRISCbits.TRISC1
#define SS_LAT          LATCbits.LATC1
#define ASSERT_SS()     SS_LAT=0
#define DEASSERT_SS()   SS_LAT=1

#define DISPLAY_SECTION_A   (~(0x01))
#define DISPLAY_SECTION_B   (~(0x02))
#define DISPLAY_SECTION_C   (~(0x04))
#define DISPLAY_SECTION_D   (~(0x08))
#define DISPLAY_SECTION_E   (~(0x10))
#define DISPLAY_SECTION_F   (~(0x20))
#define DISPLAY_SECTION_G   (~(0x40))

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

typedef struct {
    uint8_t buffer1[4];
    uint8_t buffer2[4];
    uint8_t digit;
}video_buffer_t;


extern video_buffer_t video_buffer;

void InitApp(void);         /* I/O and Peripheral Initialization */

void inline SPI_SendByte(uint8_t byte);

void Display_Clear(void);
void Display_PrintInteger(uint16_t num);