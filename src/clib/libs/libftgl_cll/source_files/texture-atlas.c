/* ============================================================================
 * Freetype GL - A C OpenGL Freetype engine
 * Platform:    Any
 * WWW:         http://code.google.com/p/freetype-gl/
 * ----------------------------------------------------------------------------
 * Copyright 2011,2012 Nicolas P. Rougier. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY NICOLAS P. ROUGIER ''AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL NICOLAS P. ROUGIER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Nicolas P. Rougier.
 * ============================================================================
 */
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

@begin
include "opengl.h"
include "texture-atlas.h"
@end

// ------------------------------------------------------ texture_atlas_new ---
texture_atlas_t *
texture_atlas_new( const int width,
                   const int height,
                   const int depth )
{/*{{{*/
  texture_atlas_t *self = (texture_atlas_t *) malloc( sizeof(texture_atlas_t) );

  // We want a one pixel border around the whole atlas to avoid any artefact when
  // sampling texture
  ftivec3 node = {{1,1,width-2}};

  assert( (depth == 1) || (depth == 3) || (depth == 4) );

  self->nodes = vector_new( sizeof(ftivec3) );
  self->used = 0;
  self->width = width;
  self->height = height;
  self->depth = depth;
  self->id = 0;

  vector_push_back( self->nodes, &node );
  self->data = (unsigned char *)
    calloc( width*height*depth, sizeof(unsigned char) );

  return self;
}/*}}}*/

// --------------------------------------------------- texture_atlas_delete ---
void
texture_atlas_delete( texture_atlas_t *self )
{/*{{{*/
  assert( self );
  vector_delete( self->nodes );
  if( self->data )
  {
    free( self->data );
  }
  if( self->id )
  {
    glDeleteTextures( 1, &self->id );
  }
  free( self );
}/*}}}*/

// ----------------------------------------------- texture_atlas_set_region ---
void
texture_atlas_set_region( texture_atlas_t * self,
                          const int x,
                          const int y,
                          const int width,
                          const int height,
                          const unsigned char * data,
                          const int stride )
{/*{{{*/
  int i;
  int depth;
  int charsize;

  assert( self );
  assert( x > 0);
  assert( y > 0);
  assert( x < (self->width-1));
  assert( (x + width) <= (self->width-1));
  assert( y < (self->height-1));
  assert( (y + height) <= (self->height-1));

  depth = self->depth;
  charsize = sizeof(char);
  for( i=0; i<height; ++i )
  {
    memcpy( self->data+((y+i)*self->width + x ) * charsize * depth,
        data + (i*stride) * charsize, width * charsize * depth  );
  }
}/*}}}*/

// ------------------------------------------------------ texture_atlas_fit ---
int
texture_atlas_fit( texture_atlas_t * self,
                   const int index,
                   const int width,
                   const int height )
{/*{{{*/
  ftivec3 *node;
  int x;
  int y;
  int width_left;
  int i;

  assert( self );

  node = (ftivec3 *) (vector_get( self->nodes, index ));
  x = node->x;
  y = node->y;
  width_left = width;
  i = index;

  if ( (x + width) > (self->width-1) )
  {
    return -1;
  }

  while( width_left > 0 )
  {
    node = (ftivec3 *) (vector_get( self->nodes, i ));
    if( node->y > y )
    {
      y = node->y;
    }
    if( (y + height) > (self->height-1) )
    {
      return -1;
    }
    width_left -= node->z;
    ++i;
  }
  return y;
}/*}}}*/

// ---------------------------------------------------- texture_atlas_merge ---
void
texture_atlas_merge( texture_atlas_t * self )
{/*{{{*/
  ftivec3 *node;
  ftivec3 *next;
  int i;

  assert( self );

  for( i=0; i< self->nodes->size-1; ++i )
  {
    node = (ftivec3 *) (vector_get( self->nodes, i ));
    next = (ftivec3 *) (vector_get( self->nodes, i+1 ));
    if( node->y == next->y )
    {
      node->z += next->z;
      vector_erase( self->nodes, i+1 );
      --i;
    }
  }
}/*}}}*/

// ----------------------------------------------- texture_atlas_get_region ---
ivec4
texture_atlas_get_region( texture_atlas_t * self,
                          const int width,
                          const int height )
{/*{{{*/
  int y;
  int best_height;
  int best_width;
  int best_index;
  ftivec3 *node;
  ftivec3 *prev;
  ivec4 region = {{0,0,width,height}};
  int i;

  assert( self );

  best_height = INT_MAX;
  best_index  = -1;
  best_width = INT_MAX;
  for( i=0; i<self->nodes->size; ++i )
  {
    y = texture_atlas_fit( self, i, width, height );
    if( y >= 0 )
    {
      node = (ftivec3 *) vector_get( self->nodes, i );
      if( ( (y + height) < best_height ) ||
          ( ((y + height) == best_height) && (node->z < best_width)) )
      {
        best_height = y + height;
        best_index = i;
        best_width = node->z;
        region.x = node->x;
        region.y = y;
      }
    }
  }

  if( best_index == -1 )
  {
    region.x = -1;
    region.y = -1;
    region.width = 0;
    region.height = 0;
    return region;
  }

  node = (ftivec3 *) malloc( sizeof(ftivec3) );
  node->x = region.x;
  node->y = region.y + height;
  node->z = width;
  vector_insert( self->nodes, best_index, node );
  free( node );

  for(i = best_index+1; i < self->nodes->size; ++i)
  {
    node = (ftivec3 *) vector_get( self->nodes, i );
    prev = (ftivec3 *) vector_get( self->nodes, i-1 );

    if (node->x < (prev->x + prev->z) )
    {
      int shrink = prev->x + prev->z - node->x;
      node->x += shrink;
      node->z -= shrink;
      if (node->z <= 0)
      {
        vector_erase( self->nodes, i );
        --i;
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
  }
  texture_atlas_merge( self );
  self->used += width * height;
  return region;
}/*}}}*/

// ---------------------------------------------------- texture_atlas_clear ---
void
texture_atlas_clear( texture_atlas_t * self )
{/*{{{*/
  ftivec3 node = {{1,1,1}};

  assert( self );
  assert( self->data );

  vector_clear( self->nodes );
  self->used = 0;
  // We want a one pixel border around the whole atlas to avoid any artefact when
  // sampling texture
  node.z = self->width-2;

  vector_push_back( self->nodes, &node );
  memset( self->data, 0, self->width*self->height*self->depth );
}/*}}}*/

// --------------------------------------------------- texture_atlas_upload ---
void
texture_atlas_upload( texture_atlas_t * self )
{/*{{{*/
  assert( self );
  assert( self->data );

  if( !self->id )
  {
    glGenTextures( 1, &self->id );
  }

  glBindTexture( GL_TEXTURE_2D, self->id );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  if( self->depth == 4 )
  {
#ifdef GL_UNSIGNED_INT_8_8_8_8_REV
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, self->width, self->height,
        0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, self->data );
#else
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, self->width, self->height,
        0, GL_RGBA, GL_UNSIGNED_BYTE, self->data );
#endif
  }
  else if( self->depth == 3 )
  {
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, self->width, self->height,
        0, GL_RGB, GL_UNSIGNED_BYTE, self->data );
  }
  else
  {
    glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA, self->width, self->height,
        0, GL_ALPHA, GL_UNSIGNED_BYTE, self->data );
  }
}/*}}}*/

