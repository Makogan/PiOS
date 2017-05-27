#include "mailbox.h"

volatile struct Message led_message =
{
  .messageSize = sizeof(struct Message),
  .requestCode =0,
  .tagID = 0x00038041,
  .bufferSize = 8,
  .requestSize =0,
  .pinNum = 130,
  .on_off_switch = 1,
  .end = 0,
};

void write_to_mailbox(uint32_t message)
{
  uint32_t status;

  do
    status = *(volatile uint32_t *)(MAIL_BASE + REGISTERS_BASE + 0x18);
  while((status & 0x80000000));

  *(volatile uint32_t *)(MAIL_BASE + REGISTERS_BASE + 0x20) = ((uint32_t)(message) & 0xfffffff0) | (uint32_t)(8);
}

uint32_t read_from_mailbox(uint32_t channel)
{
  uint32_t status;

  do
    status = *(volatile uint32_t *)(MAIL_BASE + REGISTERS_BASE + 0x18);
  while((status & 0x40000000));

  uint32_t response;

  do
    response = (*(uint32_t *)(MAIL_BASE + REGISTERS_BASE)) & 0xF;
  while(response != channel);

  return response;
}

void set_LED(int value)
{
  led_message.on_off_switch = (uint32_t) value;
  led_message.requestCode = 0;
  led_message.requestSize = 0;
  led_message.pinNum = 130;
  led_message.end = 0;

  write_to_mailbox(((uint32_t) &led_message));
}
