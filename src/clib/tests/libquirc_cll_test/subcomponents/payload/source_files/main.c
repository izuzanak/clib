
@begin
include "main.h"
@end

// === global functions ========================================================

int process_jpeg_file(const char *a_path,bc_array_s *a_trg)
{/*{{{*/
  CONT_INIT_CLEAR(image_s,img_src);
  CONT_INIT_CLEAR(image_s,img_gray);

  // - read jpeg image, convert it to grayscale -
  if (image_s_read_jpeg_file(&img_src,a_path) ||
      image_s_create(&img_gray,img_src.width,img_src.height,c_image_pixel_format_GRAY8) ||
      image_s_io_convert(&img_gray,&img_src))
  {
    throw_error(QUIRC_PAYLOAD_READ_JPEG_DATA_ERROR);
  }

  const char *buff_data = (const char *)img_gray.image_data_ptr->data;
  unsigned buff_size = img_gray.height*img_gray.image_data_ptr->line_bytes;

  CONT_INIT_CLEAR(quirc_s,quirc);

  // - search for qr codes -
  if (quirc_s_create(&quirc,img_gray.image_data_ptr->line_bytes,img_gray.height) ||
      quirc_s_process(&quirc,buff_data,buff_size))
  {
    throw_error(QUIRC_PAYLOAD_QUIRC_PROCESS_ERROR);
  }

  quirc_code_s code;
  quirc_data_s data;

  int count = quirc_s_count(&quirc);
  if (count > 0)
  {
    int idx = 0;
    do {

      // - extract and decode qr code -
      if (!(quirc_s_extract(&quirc,idx,&code) ||
            quirc_code_s_decode(&code,&data)))
      {
        quirc_data_s_payload(&data,a_trg);

        break;
      }
    } while(++idx < count);
  }

  return 0;
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();

  {
    // - ERROR -
    if (argc < 2)
    {
      return 2;
    }

    CONT_INIT_CLEAR(bc_array_s,buffer);

    // - ERROR -
    if (process_jpeg_file(argv[1],&buffer))
    {
      return 1;
    }

    // - print payload to standard output -
    printf("%.*s\n",buffer.used,buffer.data);
  }

  memcheck_release_assert();

  return 0;
}/*}}}*/

