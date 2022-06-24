#if !defined(IO_READ) && !defined(IO_MMAP) && !defined(IO_USERPTR)
#error You have to include one of IO_READ, IO_MMAP oder IO_USERPTR!
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include "gtktools.c"

#define CLEAR(x) memset (&(x), 0, sizeof (x))

typedef enum {
#ifdef IO_READ
        IO_METHOD_READ,
#endif
#ifdef IO_MMAP
        IO_METHOD_MMAP,
#endif
#ifdef IO_USERPTR
        IO_METHOD_USERPTR,
#endif
} io_method;

struct buffer {
        void *                  start;
        size_t                  length;
};

static io_method        io              = IO_METHOD_MMAP;
static int              fd              = -1;
struct buffer *         buffers         = NULL;
static unsigned int     n_buffers       = 0;

// global settings
static unsigned int width = 640;
static unsigned int height = 480;
static unsigned char jpegQuality = 100;
static char* jpegFilename = NULL;
static char* deviceName;

//global declaration
  unsigned char* src = NULL;
  #define size  640*480*3
  char dst[size];

static void YUV422toRGB888(int width, int height, unsigned char *src, unsigned char *dst)
{
  int line, column;
  unsigned char *py, *pu, *pv;
  unsigned char *tmp = dst;

  /* In this format each four bytes is two pixels. Each four bytes is two Y's, a Cb and a Cr. 
     Each Y goes to one of the pixels, and the Cb and Cr belong to both pixels. */
  py = src;
  pu = src + 1;
  pv = src + 3;

  #define CLIP(x) ( (x)>=0xFF ? 0xFF : ( (x) <= 0x00 ? 0x00 : (x) ) )

  for (line = 0; line < height; ++line) {
    for (column = 0; column < width; ++column) {
      *tmp++ = CLIP((double)*py + 1.402*((double)*pv-128.0));
      *tmp++ = CLIP((double)*py - 0.344*((double)*pu-128.0) - 0.714*((double)*pv-128.0));      
      *tmp++ = CLIP((double)*py + 1.772*((double)*pu-128.0));

      // increase py every time
      py += 2;
      // increase pu,pv every second time
      if ((column & 1)==1) {
        pu += 4;
        pv += 4;
      }
    }
  }
}


static void errno_exit(const char* s)
{
  fprintf(stderr, "%s error %d, %s\n", s, errno, strerror (errno));
  exit(EXIT_FAILURE);
}

static int xioctl(int fd, int request, void* argp)
{
  int r;

  do r = ioctl(fd, request, argp);
  while (-1 == r && EINTR == errno);

  return r;
}


static void jpegWrite(unsigned char* img)
{ 
  unsigned long location = 0;
  unsigned char* row_pointer[1];
  
  unsigned char image1[(width*height*3)];
  
  int scan = 0;
  while (scan < height) {
	  scan++;
    row_pointer[0] = &img[scan * width *  3];
    for( int i=0; i<width*3;i++){
		image1[location++] = row_pointer[0][i];}
  }

  unsigned char image[height][width][4];
  
  int i, j;
  int k = 0;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
			k++;
			tmpresultX[k] = j;
			tmpresultY[k] = i;
            image[height-i][j][2] = (image1[(i*width*3)+(j*3)+0]);           ///red
            image[height-i][j][1] = (image1[(i*width*3)+(j*3)+1]);             ///green
            image[height-i][j][0] = (image1[(i*width*3)+(j*3)+2]); ///blue
            
            resultR[i][j] = (int)(image[height-i][j][2]);
			resultG[i][j] = (int)(image[height-i][j][1]);
			resultB[i][j] = (int)(image[height-i][j][0]);
        }
    }
 
   msize = k;
}

static void imageProcess(const void* p)
{
  src = (unsigned char*)p;

  // convert from YUV422 to RGB888
  YUV422toRGB888(width,height,src,dst);

  // write jpeg
  jpegWrite(dst);
  
  // free temporary image
  //~ free(dst);
}

static int frameRead(void)
{
  struct v4l2_buffer buf;
#ifdef IO_USERPTR
  unsigned int i;
#endif

  switch (io) {
#ifdef IO_READ
    case IO_METHOD_READ:
      if (-1 == read (fd, buffers[0].start, buffers[0].length)) {
        switch (errno) {
          case EAGAIN:
            return 0;

          case EIO:
            // Could ignore EIO, see spec.

            // fall through
          default:
            errno_exit("read");
        }
      }

      imageProcess(buffers[0].start);
      break;
#endif

#ifdef IO_MMAP
    case IO_METHOD_MMAP:
      CLEAR (buf);

      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory = V4L2_MEMORY_MMAP;

      if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
        switch (errno) {
          case EAGAIN:
            return 0;

          case EIO:
             // Could ignore EIO, see spec

             // fall through
          default:
            errno_exit("VIDIOC_DQBUF");
        }
      }

      assert (buf.index < n_buffers);

      imageProcess(buffers[buf.index].start);

      if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
        errno_exit("VIDIOC_QBUF");

      break;
#endif

#ifdef IO_USERPTR
    case IO_METHOD_USERPTR:
      CLEAR (buf);

      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory = V4L2_MEMORY_USERPTR;

      if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
        switch (errno) {
          case EAGAIN:
            return 0;

          case EIO:
            // Could ignore EIO, see spec.

            // fall through
          default:
            errno_exit("VIDIOC_DQBUF");
                        
        }
      }

      for (i = 0; i < n_buffers; ++i)
        if (buf.m.userptr == (unsigned long) buffers[i].start && buf.length == buffers[i].length)
          break;

      assert (i < n_buffers);

      imageProcess((void *) buf.m.userptr);

      if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
        errno_exit("VIDIOC_QBUF");
      break;
#endif

    }

  return 1;
}

static void mainLoop(void)
{
  unsigned int count;

  count = 1;

  while (count-- > 0) {
    for (;;) {
      fd_set fds;
      struct timeval tv;
      int r;

      FD_ZERO(&fds);
      FD_SET(fd, &fds);

      /* Timeout. */
      tv.tv_sec = 2;
      tv.tv_usec = 0;

      r = select(fd + 1, &fds, NULL, NULL, &tv);

      if (-1 == r) {
        if (EINTR == errno)
          continue;

        errno_exit("select");
      }

      if (0 == r) {
        fprintf (stderr, "select timeout\n");
        exit(EXIT_FAILURE);
      }

      if (frameRead())
        break;
        
      /* EAGAIN - continue select loop. */
    }
  }
}

static void captureStop(void)
{
  enum v4l2_buf_type type;

  switch (io) {
#ifdef IO_READ
    case IO_METHOD_READ:
      /* Nothing to do. */
      break;
#endif

#ifdef IO_MMAP
    case IO_METHOD_MMAP:
#endif
#ifdef IO_USERPTR
    case IO_METHOD_USERPTR:
#endif
#if defined(IO_MMAP) || defined(IO_USERPTR)
      type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

      if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
        errno_exit("VIDIOC_STREAMOFF");

      break;
#endif 
   }
}

static void captureStart(void)
{
  unsigned int i;
  enum v4l2_buf_type type;

  switch (io) {
#ifdef IO_READ    
    case IO_METHOD_READ:
      /* Nothing to do. */
      break;
#endif

#ifdef IO_MMAP
    case IO_METHOD_MMAP:
      for (i = 0; i < n_buffers; ++i) {
        struct v4l2_buffer buf;

        CLEAR (buf);

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = i;

        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
          errno_exit("VIDIOC_QBUF");
      }
                
      type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

      if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
        errno_exit("VIDIOC_STREAMON");

      break;
#endif

#ifdef IO_USERPTR
    case IO_METHOD_USERPTR:
      for (i = 0; i < n_buffers; ++i) {
        struct v4l2_buffer buf;

        CLEAR (buf);

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_USERPTR;
        buf.index       = i;
        buf.m.userptr   = (unsigned long) buffers[i].start;
        buf.length      = buffers[i].length;

        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
          errno_exit("VIDIOC_QBUF");
      }

      type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

      if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
         errno_exit("VIDIOC_STREAMON");

      break;
#endif
  }
}

static void deviceUninit(void)
{
  unsigned int i;

  switch (io) {
#ifdef IO_READ
    case IO_METHOD_READ:
      free(buffers[0].start);
      break;
#endif

#ifdef IO_MMAP
    case IO_METHOD_MMAP:
      for (i = 0; i < n_buffers; ++i)
      if (-1 == munmap (buffers[i].start, buffers[i].length))
        errno_exit("munmap");
      break;
#endif

#ifdef IO_USERPTR
    case IO_METHOD_USERPTR:
      for (i = 0; i < n_buffers; ++i)
        free (buffers[i].start);
      break;
#endif
  }

  free(buffers);
}

#ifdef IO_READ
static void readInit(unsigned int buffer_size)
{
  buffers = calloc(1, sizeof(*buffers));

  if (!buffers) {
    fprintf (stderr, "Out of memory\n");
    exit(EXIT_FAILURE);
  }

  buffers[0].length = buffer_size;
  buffers[0].start = malloc (buffer_size);

  if (!buffers[0].start) {
    fprintf (stderr, "Out of memory\n");
    exit(EXIT_FAILURE);
  }
}
#endif

#ifdef IO_MMAP
static void mmapInit(void)
{
  struct v4l2_requestbuffers req;

  CLEAR (req);

  req.count               = 4;
  req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory              = V4L2_MEMORY_MMAP;

  if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
    if (EINVAL == errno) {
      fprintf(stderr, "%s does not support memory mapping\n", deviceName);
      exit(EXIT_FAILURE);
    } else {
      errno_exit("VIDIOC_REQBUFS");
    }
  }

  if (req.count < 2) {
    fprintf(stderr, "Insufficient buffer memory on %s\n", deviceName);
    exit(EXIT_FAILURE);
  }

  buffers = calloc(req.count, sizeof(*buffers));

  if (!buffers) {
    fprintf(stderr, "Out of memory\n");
    exit(EXIT_FAILURE);
  }

  for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
    struct v4l2_buffer buf;

    CLEAR (buf);

    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = n_buffers;

    if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
      errno_exit("VIDIOC_QUERYBUF");

    buffers[n_buffers].length = buf.length;
    buffers[n_buffers].start =
    mmap (NULL /* start anywhere */, buf.length, PROT_READ | PROT_WRITE /* required */, MAP_SHARED /* recommended */, fd, buf.m.offset);

    if (MAP_FAILED == buffers[n_buffers].start)
      errno_exit("mmap");
  }
}
#endif

#ifdef IO_USERPTR
static void userptrInit(unsigned int buffer_size)
{
  struct v4l2_requestbuffers req;
  unsigned int page_size;

  page_size = getpagesize ();
  buffer_size = (buffer_size + page_size - 1) & ~(page_size - 1);

  CLEAR(req);

  req.count               = 4;
  req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory              = V4L2_MEMORY_USERPTR;

  if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
    if (EINVAL == errno) {
      fprintf(stderr, "%s does not support user pointer i/o\n", deviceName);
      exit(EXIT_FAILURE);
    } else {
      errno_exit("VIDIOC_REQBUFS");
    }
  }

  buffers = calloc(4, sizeof (*buffers));

  if (!buffers) {
    fprintf(stderr, "Out of memory\n");
    exit(EXIT_FAILURE);
  }

  for (n_buffers = 0; n_buffers < 4; ++n_buffers) {
    buffers[n_buffers].length = buffer_size;
    buffers[n_buffers].start = memalign (/* boundary */ page_size, buffer_size);

    if (!buffers[n_buffers].start) {
      fprintf(stderr, "Out of memory\n");
      exit(EXIT_FAILURE);
    }
  }
}
#endif

static void deviceInit(void)
{
  struct v4l2_capability cap;
  struct v4l2_cropcap cropcap;
  struct v4l2_crop crop;
  struct v4l2_format fmt;
  unsigned int min;

  if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
    if (EINVAL == errno) {
      fprintf(stderr, "%s is no V4L2 device\n",deviceName);
      exit(EXIT_FAILURE);
    } else {
      errno_exit("VIDIOC_QUERYCAP");
    }
  }

  if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
    fprintf(stderr, "%s is no video capture device\n",deviceName);
    exit(EXIT_FAILURE);
  }

  switch (io) {
#ifdef IO_READ
    case IO_METHOD_READ:
      if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
        fprintf(stderr, "%s does not support read i/o\n",deviceName);
        exit(EXIT_FAILURE);
      }
      break;
#endif

#ifdef IO_MMAP
    case IO_METHOD_MMAP:
#endif
#ifdef IO_USERPTR
    case IO_METHOD_USERPTR:
#endif
#if defined(IO_MMAP) || defined(IO_USERPTR)
      if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
        fprintf(stderr, "%s does not support streaming i/o\n",deviceName);
        exit(EXIT_FAILURE);
      }
      break;
#endif
  }


  /* Select video input, video standard and tune here. */
  CLEAR(cropcap);

  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c = cropcap.defrect; /* reset to default */

    if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) {
      switch (errno) {
        case EINVAL:
          /* Cropping not supported. */
          break;
        default:
          /* Errors ignored. */
          break;
      }
    }
  } else {        
    /* Errors ignored. */
  }

  CLEAR (fmt);

  // v4l2_format
  fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width       = width; 
  fmt.fmt.pix.height      = height;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
  fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

  if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
    errno_exit("VIDIOC_S_FMT");

  /* Note VIDIOC_S_FMT may change width and height. */
  if (width != fmt.fmt.pix.width) {
    width = fmt.fmt.pix.width;
    fprintf(stderr,"Image width set to %i by device %s.\n",width,deviceName);
  }
  if (height != fmt.fmt.pix.height) {
    height = fmt.fmt.pix.height;
    fprintf(stderr,"Image height set to %i by device %s.\n",height,deviceName);
  }

  /* Buggy driver paranoia. */
  min = fmt.fmt.pix.width * 2;
  if (fmt.fmt.pix.bytesperline < min)
    fmt.fmt.pix.bytesperline = min;
  min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
  if (fmt.fmt.pix.sizeimage < min)
    fmt.fmt.pix.sizeimage = min;

  switch (io) {
#ifdef IO_READ
    case IO_METHOD_READ:
      readInit(fmt.fmt.pix.sizeimage);
      break;
#endif

#ifdef IO_MMAP
    case IO_METHOD_MMAP:
      mmapInit();
      break;
#endif

#ifdef IO_USERPTR
    case IO_METHOD_USERPTR:
      userptrInit(fmt.fmt.pix.sizeimage);
      break;
#endif
  }
}

static void deviceClose(void)
{
  if (-1 == close (fd))
    errno_exit("close");

  fd = -1;
}

static void deviceOpen(void)
{
  struct stat st;

  // stat file
  if (-1 == stat(deviceName, &st)) {
    fprintf(stderr, "Cannot identify '%s': %d, %s\n", deviceName, errno, strerror (errno));
    exit(EXIT_FAILURE);
  }

  // check if its device
  if (!S_ISCHR (st.st_mode)) {
    fprintf(stderr, "%s is no device\n", deviceName);
    exit(EXIT_FAILURE);
  }

  // open device
  fd = open(deviceName, O_RDWR /* required */ | O_NONBLOCK, 0);

  // check if opening was successfull
  if (-1 == fd) {
    fprintf(stderr, "Cannot open '%s': %d, %s\n", deviceName, errno, strerror (errno));
    exit(EXIT_FAILURE);
  }
}

color_coordinate_matrix process_frame(){
  //~ deviceName = devname;
  //~ // open and initialize device
  //~ deviceOpen();
  //~ deviceInit();

  //~ // start capturing
  //~ captureStart();

  // process frames
  mainLoop();
  
  color_coordinate_matrix ccordinate;
  
	ccordinate.matrix_R.matriks = resultR;
	ccordinate.matrix_R.cols = width;
	ccordinate.matrix_R.rows = height;
	
	ccordinate.matrix_G.matriks = resultG;
	ccordinate.matrix_G.cols = width;
	ccordinate.matrix_G.rows = height;
	
	ccordinate.matrix_B.matriks = resultB;
	ccordinate.matrix_B.cols = width;
	ccordinate.matrix_B.rows = height;
	
	ccordinate.Coordinate.X = tmpresultX;
	ccordinate.Coordinate.Y = tmpresultY;
	ccordinate.Coordinate.MSize = msize;
	

  //~ // stop capturing
  //~ captureStop();

  //~ // close device
  //~ deviceUninit();
  //~ deviceClose();
  return ccordinate;
}
