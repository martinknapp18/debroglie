/***********************************
Copyright ©2019. The Regents of the University of California (Regents). All Rights Reserved.
Permission to use, copy, modify, and distribute this software and its documentation for
educational, research, and not-for-profit purposes, without fee and without a signed licensing
agreement, is hereby granted, provided that the above copyright notice, this paragraph and the
following two paragraphs appear in all copies, modifications, and distributions. Contact The Office
of Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley, CA 94720-1620,
(510) 643-7201, otl@berkeley.edu, http://ipira.berkeley.edu/industry-info for commercial licensing
opportunities.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
***********************************/

#include "cycle_count_delay.h"
#include "mbed.h"
#include "minig.h"
#include "util/macros.h"

#if !DEVICE_FLASH
#error [NOT_SUPPORTED] Flash API not supported for this target
#endif

#define K_UP 0


#include "FlashIAP.h"

// Create flash IAP block device
FlashIAP bd;
void test_bd() {
  printf("FlashIAPBlockDevice test\n");

  // Initialize the flash IAP block device and print the memory layout
  bd.init();
  printf("Flash block device size: %lu\n", bd.get_flash_size());
  printf("Flash block device read size: %lu\n", bd.get_page_size());
  printf("Flash block device program size: %lu\n", bd.get_page_size());
  printf("Flash block device erase size: %lu\n",
         bd.get_sector_size(bd.get_flash_start()));

  // Write "Hello World!" to the first block
  char *buffer = (char *)malloc(bd.get_page_size());
  sprintf(buffer, "Hello World!\n");
  bd.erase(0, bd.get_page_size());
  bd.program(buffer, 0, bd.get_page_size());

  // Read back what was stored
  bd.read(buffer, 0, bd.get_page_size());
  printf("%s", buffer);

  // Deinitialize the device
  bd.deinit();
}

int main() {
  enableCycleCounter();
  test_bd();

#if K_UP
  MiniG minig(true);
#else
  MiniG minig(false);
#endif
  minig.init();
  while (1) {
    minig.run();
  }

  // For recording timing parameters
  //
  // while (1) {

  //   WRITE_IO(GPIOG, GPIO_PIN_2, BITS_NONE);
  //   bsm_delay_us(4);
  //   WRITE_IO(GPIOG, BITS_NONE, GPIO_PIN_2);
  //   bsm_delay_us(4);
  // }
}
