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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

@begin
include "vec234.h"
include "platform.h"
include "vertex-buffer.h"
@end

/**
 * Buffer status
 */
#define CLEAN  (0)
#define DIRTY  (1)
#define FROZEN (2)

// ----------------------------------------------------------------------------
vertex_buffer_t *
vertex_buffer_new( const char *format )
{/*{{{*/
  int i;
  int index = 0;
  int stride = 0;
  const char *start = 0;
  const char *end = 0;
  GLchar *pointer = 0;

  vertex_buffer_t *self = (vertex_buffer_t *) malloc (sizeof(vertex_buffer_t));
  self->format = strdup( format );

  for( i=0; i<MAX_VERTEX_ATTRIBUTE; ++i )
  {
    self->attributes[i] = 0;
  }

#define VERTEX_BUFFER_NEW_ERROR() \
  {/*{{{*/\
    int del_index = 0;\
    do {\
      vertex_attribute_delete(self->attributes[del_index]);\
    } while(++del_index < index);\
    \
    if (attribute != NULL)\
    {\
      vertex_attribute_delete(attribute);\
    }\
    free(self->format);\
    free(self);\
  }/*}}}*/

  start = format;
  do
  {
    char *desc = 0;
    vertex_attribute_t *attribute;
    GLuint attribute_size = 0;
    end = strchr(start+1, ',');

    if (end == NULL)
    {
      desc = strdup( start );
    }
    else
    {
      desc = strndup( start, end-start );
    }
    attribute = vertex_attribute_parse( desc );
    start = end+1;
    free(desc);

    if (attribute == NULL)
    {
      VERTEX_BUFFER_NEW_ERROR();
      return NULL;
    }

    attribute->pointer = pointer;

    switch( attribute->type )
    {
      case GL_BOOL:           attribute_size = sizeof(GLboolean); break;
      case GL_BYTE:           attribute_size = sizeof(GLbyte); break;
      case GL_UNSIGNED_BYTE:  attribute_size = sizeof(GLubyte); break;
      case GL_SHORT:          attribute_size = sizeof(GLshort); break;
      case GL_UNSIGNED_SHORT: attribute_size = sizeof(GLushort); break;
      case GL_INT:            attribute_size = sizeof(GLint); break;
      case GL_UNSIGNED_INT:   attribute_size = sizeof(GLuint); break;
      case GL_FLOAT:          attribute_size = sizeof(GLfloat); break;

                              // - ERROR -
      default:
                              VERTEX_BUFFER_NEW_ERROR();
                              return NULL;
    }
    stride  += attribute->size*attribute_size;
    pointer += attribute->size*attribute_size;
    self->attributes[index] = attribute;
    index++;
  } while ( end && (index < MAX_VERTEX_ATTRIBUTE) );

  for( i=0; i<index; ++i )
  {
    self->attributes[i]->stride = stride;
  }

  self->vertices = vector_new( stride );
  self->vertices_id  = 0;
  self->GPU_vsize = 0;

  self->indices = vector_new( sizeof(GLuint) );
  self->indices_id  = 0;
  self->GPU_isize = 0;

  self->items = vector_new( sizeof(ivec4) );
  self->state = DIRTY;
  self->mode = GL_TRIANGLES;
  return self;
}/*}}}*/

// ----------------------------------------------------------------------------
void
vertex_buffer_delete( vertex_buffer_t *self )
{/*{{{*/
  int i;

  assert( self );

  for( i=0; i<MAX_VERTEX_ATTRIBUTE; ++i )
  {
    if( self->attributes[i] )
    {
      vertex_attribute_delete( self->attributes[i] );
    }
  }

  vector_delete( self->vertices );
  self->vertices = 0;
  if( self->vertices_id )
  {
    glDeleteBuffers( 1, &self->vertices_id );
  }
  self->vertices_id = 0;

  vector_delete( self->indices );
  self->indices = 0;
  if( self->indices_id )
  {
    glDeleteBuffers( 1, &self->indices_id );
  }
  self->indices_id = 0;

  vector_delete( self->items );

  if( self->format )
  {
    free( self->format );
  }
  self->format = 0;
  self->state = 0;
  free( self );
}/*}}}*/

// ----------------------------------------------------------------------------
const char *
vertex_buffer_format( const vertex_buffer_t *self )
{/*{{{*/
  assert( self );

  return self->format;
}/*}}}*/

// ----------------------------------------------------------------------------
int
vertex_buffer_size( const vertex_buffer_t *self )
{/*{{{*/
  assert( self );

  return vector_size( self->items );
}/*}}}*/

// ----------------------------------------------------------------------------
void
vertex_buffer_print( vertex_buffer_t * self )
{/*{{{*/
  int i = 0;
  static char *gltypes[9] = {
    (char *)"GL_BOOL",
    (char *)"GL_BYTE",
    (char *)"GL_UNSIGNED_BYTE",
    (char *)"GL_SHORT",
    (char *)"GL_UNSIGNED_SHORT",
    (char *)"GL_INT",
    (char *)"GL_UNSIGNED_INT",
    (char *)"GL_FLOAT",
    (char *)"GL_VOID"
  };

  assert(self);

  fprintf( stderr, "%ld vertices, %ld indices\n",
      (long int)vector_size( self->vertices ), (long int)vector_size( self->indices ) );
  while( self->attributes[i] )
  {
    int j = 8;
    switch( self->attributes[i]->type )
    {
      case GL_BOOL:           j=0; break;
      case GL_BYTE:           j=1; break;
      case GL_UNSIGNED_BYTE:  j=2; break;
      case GL_SHORT:          j=3; break;
      case GL_UNSIGNED_SHORT: j=4; break;
      case GL_INT:            j=5; break;
      case GL_UNSIGNED_INT:   j=6; break;
      case GL_FLOAT:          j=7; break;
      default:                j=8; break;
    }
    fprintf(stderr, "%s : %dx%s (+%p)\n",
        self->attributes[i]->name,
        self->attributes[i]->size,
        gltypes[j],
        self->attributes[i]->pointer);

    i += 1;
  }
}/*}}}*/

// ----------------------------------------------------------------------------
void
vertex_buffer_upload ( vertex_buffer_t *self )
{/*{{{*/
  int vsize;
  int isize;

  if( self->state == FROZEN )
  {
    return;
  }

  if( !self->vertices_id )
  {
    glGenBuffers( 1, &self->vertices_id );
  }
  if( !self->indices_id )
  {
    glGenBuffers( 1, &self->indices_id );
  }

  vsize = self->vertices->size*self->vertices->item_size;
  isize = self->indices->size*self->indices->item_size;

  // Always upload vertices first such that indices do not point to non
  // existing data (if we get interrupted in between for example).

  // Upload vertices
  glBindBuffer( GL_ARRAY_BUFFER, self->vertices_id );
  if( vsize != self->GPU_vsize )
  {
    glBufferData( GL_ARRAY_BUFFER,
        vsize, self->vertices->items, GL_DYNAMIC_DRAW );
    self->GPU_vsize = vsize;
  }
  else
  {
    glBufferSubData( GL_ARRAY_BUFFER,
        0, vsize, self->vertices->items );
  }
  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  // Upload indices
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, self->indices_id );
  if( isize != self->GPU_isize )
  {
    glBufferData( GL_ELEMENT_ARRAY_BUFFER,
        isize, self->indices->items, GL_DYNAMIC_DRAW );
    self->GPU_isize = isize;
  }
  else
  {
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER,
        0, isize, self->indices->items );
  }
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}/*}}}*/

// ----------------------------------------------------------------------------
void
vertex_buffer_clear( vertex_buffer_t *self )
{/*{{{*/
  assert( self );

  self->state = FROZEN;
  vector_clear( self->indices );
  vector_clear( self->vertices );
  vector_clear( self->items );
  self->state = DIRTY;
}/*}}}*/

// ----------------------------------------------------------------------------
void
vertex_buffer_render_setup ( vertex_buffer_t *self, GLenum mode )
{/*{{{*/
  int i;

  if( self->state != CLEAN )
  {
    vertex_buffer_upload( self );
    self->state = CLEAN;
  }

  glBindBuffer( GL_ARRAY_BUFFER, self->vertices_id );

  for( i=0; i<MAX_VERTEX_ATTRIBUTE; ++i )
  {
    vertex_attribute_t *attribute = self->attributes[i];
    if ( attribute == 0 )
    {
      continue;
    }

    vertex_attribute_enable( attribute );
  }

  if( self->indices->size )
  {
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, self->indices_id );
  }

  self->mode = mode;
}/*}}}*/

// ----------------------------------------------------------------------------
void
vertex_buffer_render_finish ( vertex_buffer_t *self )
{/*{{{*/
  (void)self;

  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}/*}}}*/

// ----------------------------------------------------------------------------
void
vertex_buffer_render_item ( vertex_buffer_t *self,
                            int index )
{/*{{{*/
  ivec4 * item = (ivec4 *) vector_get( self->items, index );
  assert( self );
  assert( index < vector_size( self->items ) );

  if( self->indices->size )
  {
    int start = item->istart;
    int count = item->icount;
    glDrawElements( self->mode, count, GL_UNSIGNED_INT, (void *)(start*sizeof(GLuint)) );
  }
  else if( self->vertices->size )
  {
    int start = item->vstart;
    int count = item->vcount;
    glDrawArrays( self->mode, start*self->vertices->item_size, count);
  }
}/*}}}*/

// ----------------------------------------------------------------------------
void
vertex_buffer_render ( vertex_buffer_t *self, GLenum mode )
{/*{{{*/
  int vcount = self->vertices->size;
  int icount = self->indices->size;

  vertex_buffer_render_setup( self, mode );
  if( icount )
  {
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, self->indices_id );
    glDrawElements( mode, icount, GL_UNSIGNED_INT, 0 );
  }
  else
  {
    glDrawArrays( mode, 0, vcount );
  }
  vertex_buffer_render_finish( self );
}/*}}}*/

// ----------------------------------------------------------------------------
void
vertex_buffer_push_back_indices ( vertex_buffer_t * self,
                                  const GLuint * indices,
                                  const int icount )
{/*{{{*/
  assert( self );

  self->state |= DIRTY;
  vector_push_back_data( self->indices, indices, icount );
}/*}}}*/

// ----------------------------------------------------------------------------
void
vertex_buffer_push_back_vertices ( vertex_buffer_t * self,
                                   const void * vertices,
                                   const int vcount )
{/*{{{*/
  assert( self );

  self->state |= DIRTY;
  vector_push_back_data( self->vertices, vertices, vcount );
}/*}}}*/

// ----------------------------------------------------------------------------
void
vertex_buffer_insert_indices ( vertex_buffer_t *self,
                               const int index,
                               const GLuint *indices,
                               const int count )
{/*{{{*/
  assert( self );
  assert( self->indices );
  assert( index < self->indices->size+1 );

  self->state |= DIRTY;
  vector_insert_data( self->indices, index, indices, count );
}/*}}}*/

// ----------------------------------------------------------------------------
void
vertex_buffer_insert_vertices( vertex_buffer_t *self,
                               const int index,
                               const void *vertices,
                               const int vcount )
{/*{{{*/
  int i;
  assert( self );
  assert( self->vertices );
  assert( index < self->vertices->size+1 );

  self->state |= DIRTY;

  for( i=0; i<self->indices->size; ++i )
  {
    if( *(GLuint *)(vector_get( self->indices, i )) > (unsigned)index )
    {
      *(GLuint *)(vector_get( self->indices, i )) += index;
    }
  }

  vector_insert_data( self->vertices, index, vertices, vcount );
}/*}}}*/

// ----------------------------------------------------------------------------
void
vertex_buffer_erase_indices( vertex_buffer_t *self,
                             const int first,
                             const int last )
{/*{{{*/
  assert( self );
  assert( self->indices );
  assert( first < self->indices->size );
  assert( (last) <= self->indices->size );

  self->state |= DIRTY;
  vector_erase_range( self->indices, first, last );
}/*}}}*/

// ----------------------------------------------------------------------------
void
vertex_buffer_erase_vertices( vertex_buffer_t *self,
                              const int first,
                              const int last )
{/*{{{*/
  int i;
  assert( self );
  assert( self->vertices );
  assert( first < self->vertices->size );
  assert( (first+last) <= self->vertices->size );
  assert( last > first );

  self->state |= DIRTY;
  for( i=0; i<self->indices->size; ++i )
  {
    if( *(GLuint *)(vector_get( self->indices, i )) > (unsigned)first )
    {
      *(GLuint *)(vector_get( self->indices, i )) -= (last-first);
    }
  }
  vector_erase_range( self->vertices, first, last );
}/*}}}*/

// ----------------------------------------------------------------------------
int
vertex_buffer_push_back( vertex_buffer_t * self,
                         const void * vertices, const int vcount,
                         const GLuint * indices, const int icount )
{/*{{{*/
  return vertex_buffer_insert( self, vector_size( self->items ),
      vertices, vcount, indices, icount );
}/*}}}*/

// ----------------------------------------------------------------------------
int
vertex_buffer_insert( vertex_buffer_t * self, const int index,
                      const void * vertices, const int vcount,
                      const GLuint * indices, const int icount )
{/*{{{*/
  int vstart;
  int istart;
  int i;
  ivec4 item;
  assert( self );
  assert( vertices );
  assert( indices );

  self->state = FROZEN;

  // Push back vertices
  vstart = vector_size( self->vertices );
  vertex_buffer_push_back_vertices( self, vertices, vcount );

  // Push back indices
  istart = vector_size( self->indices );
  vertex_buffer_push_back_indices( self, indices, icount );

  // Update indices within the vertex buffer
  for( i=0; i<icount; ++i )
  {
    *(GLuint *)(vector_get( self->indices, istart+i )) += vstart;
  }

  // Insert item
  item.x = vstart;
  item.y = vcount;
  item.z = istart;
  item.w = icount;
  vector_insert( self->items, index, &item );

  self->state = DIRTY;
  return index;
}/*}}}*/

// ----------------------------------------------------------------------------
void
vertex_buffer_erase( vertex_buffer_t * self,
                     const int index )
{/*{{{*/
  ivec4 * item;
  int vstart;
  int vcount;
  int istart;
  int icount;
  int i;

  assert( self );
  assert( index < vector_size( self->items ) );

  item = (ivec4 *) vector_get( self->items, index );
  vstart = item->vstart;
  vcount = item->vcount;
  istart = item->istart;
  icount = item->icount;

  // Update items
  for( i=0; i<vector_size(self->items); ++i )
  {
    ivec4 * item = (ivec4 *) vector_get( self->items, i );
    if( item->vstart > vstart)
    {
      item->vstart -= vcount;
      item->istart -= icount;
    }
  }

  self->state = FROZEN;
  vertex_buffer_erase_indices( self, istart, istart+icount );
  vertex_buffer_erase_vertices( self, vstart, vstart+vcount );
  vector_erase( self->items, index );
  self->state = DIRTY;
}/*}}}*/

