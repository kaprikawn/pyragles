#ifndef JSON_HPP
#define JSON_HPP

#include "types.hpp"
#include "strings.hpp"
#include "object_data.hpp"

#ifdef _WIN32

#pragma pack(1)
struct  GltfHeader {
  u32 magic;
  u32 version;
  u32 length;
  u32 json_chunk_length;
  u32 json_chunk_type;
};

#elif __linux__

struct __attribute__ ((__packed__)) GltfHeader {
  u32 magic;
  u32 version;
  u32 length;
  u32 json_chunk_length;
  u32 json_chunk_type;
};

#endif

// see https://en.cppreference.com/w/cpp/language/ascii
enum ascii_chars {ASCII_NUL,ASCII_SOH,ASCII_STX,ASCII_ETX,ASCII_EOT,ASCII_ENQ,ASCII_ACK,ASCII_BEL,ASCII_BS,ASCII_HT,ASCII_LF,ASCII_VT,ASCII_FF,ASCII_CR,ASCII_SO,ASCII_SI,ASCII_DLE,ASCII_DC1,ASCII_DC2,ASCII_DC3,ASCII_DC4,ASCII_NAK,ASCII_SYN,ASCII_ETB,ASCII_CAN,ASCII_EM,ASCII_SUB,ASCII_ESC,ASCII_FS,ASCII_GS,ASCII_RS,ASCII_US,ASCII_SPACE,ASCII_EXCLAMATION,ASCII_DOUBLE_QUOTE,ASCII_HASH,ASCII_DOLLAR,ASCII_PERCENTAGE,ASCII_AMPERSANDS,ASCII_APOSTROPHE,ASCII_OPEN_BRACKET,ASCII_CLOSE_BRACKET,ASCII_STAR,ASCII_UNK1,ASCII_COMMA,ASCII_HYPHEN,ASCII_DECIMAL_POINT,ASCII_FORWARD_SLASH,ASCII_0,ASCII_1,ASCII_2,ASCII_3,ASCII_4,ASCII_5,ASCII_6,ASCII_7,ASCII_8,ASCII_9,ASCII_COLON,ASCII_SEMI_COLON,ASCII_LESS_THAN,ASCII_EQUALS,ASCII_GREATER_THAN,ASCII_QUESTION_MARK,ASCII_AT_SYMBOL,ASCII_A,ASCII_B,ASCII_C,ASCII_D,ASCII_E,ASCII_F,ASCII_G,ASCII_H,ASCII_I,ASCII_J,ASCII_K,ASCII_L,ASCII_M,ASCII_N,ASCII_O,ASCII_P,ASCII_Q,ASCII_R,ASCII_S,ASCII_T,ASCII_U,ASCII_V,ASCII_W,ASCII_X,ASCII_Y,ASCII_Z,ASCII_OPEN_SQUARE_BRACKETS,ASCII_BACK_SLASH,ASCII_CLOSE_SQUARE_BRACKETS,ASCII_CARROT,ASCII_UNDERSCORE,ASCII_BACKTICK,ASCII_a,ASCII_b,ASCII_c,ASCII_d,ASCII_e,ASCII_f,ASCII_g,ASCII_h,ASCII_i,ASCII_j,ASCII_k,ASCII_l,ASCII_m,ASCII_n,ASCII_o,ASCII_p,ASCII_q,ASCII_r,ASCII_s,ASCII_t,ASCII_u,ASCII_v,ASCII_w,ASCII_x,ASCII_y,ASCII_z,ASCII_OPEN_CURLY_BRACKET,ASCII_PIPE,ASCII_CLOSE_CURLY_BRACKET,ASCII_TILDE,ASCII_DEL};

enum current_positions { JSON_UNK, JSON_ASSET, JSON_SCENES, JSON_NODES, JSON_MATERIALS, JSON_MESHES, JSON_TEXTURES, JSON_IMAGES, JSON_ACCESSORS, JSON_BUFFER_VIEWS, JSON_BUFFERS };
enum json_element_types { JSON_ELEMENT_NONE, JSON_ELEMENT_BRACKET, JSON_ELEMENT_CHAR, JSON_ELEMENT_NUMBER, JSON_ELEMENT_COMMA, JSON_ELEMENT_DOUBLE_QUOTE, JSON_ELEMENT_COLON };
enum json_states { JSON_STATE_NONE, JSON_STATE_READING_KEY, JSON_STATE_READING_VALUE, JSON_STATE_READING_JSON };
enum GltfContentType { GLTF_VERTICES, GLTF_INDICES, GLTF_NORMALS, GLTF_TEX_COORD0, GLTF_IMAGE };
enum AccessorType { ACCESSOR_VEC3, ACCESSOR_VEC2, ACCESSOR_SCALAR };

struct MeshData {
  bool32  is_collider = false;
  u32     vertex_data_in_bytes;
  u32     vertex_count;
  u32     index_data_raw_in_bytes;
  u32     index_data_in_bytes;
  u32     index_count;
  u32     normal_data_in_bytes;
  u32     normal_count;
  u32     tex_coord0_data_in_bytes;
  u32     tex_coord0_count;
  u32     image_data_in_bytes;
  f32*    vertex_data;
  f32*    normal_data;
  u16*    index_data_raw;
  u32*    index_data;
  f32*    tex_coord0_data;
  u8*     image_data;
  u32     gl_vertex_offset;
  u32     gl_normal_offset;
  u32     gl_tex_coord0_offset;
  u32     gl_index_offset;
};

struct JsonString {
  u32   json_char_count;
  char* json_string;
};

struct GltfbufferViewInfo {
  u32 buffer_view;
  u32 component_type;
  u32 gltf_count; // count as defined by the JSON
  u32 buffer;
  u32 byte_length;
  u32 byte_offset;
  u32 count; // count of values
  u32 offset;
  s32 type;
};

struct MeshPositionIndices {
  u32 vertices;
  u32 normals;
  u32 texcoord_0;
  u32 indices;
  u32 material;
};

struct AccessorData {
  u32   buffer_view;
  u32   component_type;
  u32   count;
  s32   accessor_type;
  char  type[ 7 ];
};

struct BufferViewData {
  u32 buffer;
  u32 byte_length;
  u32 byte_offset;
};

inline u32 json_size_in_bytes( ReadFileResult* gltf_file ) {
  GltfHeader* gltf_header = ( GltfHeader* )gltf_file -> contents;
  return gltf_header -> json_chunk_length;
}

inline u32 get_bin_start_offset(  GltfHeader* gltf_header ) {
  u32 result;
  u32 gltf_header_size_in_bytes = 12; // magic + version + length
  u32 json_header_in_bytes      = 8; // 4 bytes for chunk length, 4 bytes for chunk type
  u32 json_string_in_bytes      = gltf_header -> json_chunk_length;
  u32 bin_header_in_bytes       = 8; // 4 bytes for chunk length, 4 bytes for chunk type
  result                        = gltf_header_size_in_bytes + json_header_in_bytes + json_string_in_bytes + bin_header_in_bytes;
  return result;
}

inline void pull_out_json_string( ReadFileResult* gltf_file, char* json_string, u32 json_string_length ) {
  
  GltfHeader* gltf_header               = ( GltfHeader* )gltf_file -> contents;
  
  u8          file_header_size_in_bytes = 12; // magic + version + length
  u8          json_header_size_in_bytes = 8; // 4 bytes for chunk length, 4 bytes for chunk type
  u32         jsonCurrentIndex          = 0;
  const char* json_data = ( char* )( ( u8* )gltf_file -> contents + file_header_size_in_bytes + json_header_size_in_bytes );
  for( u32 i = 0; i < json_string_length; i++ ) {
    json_string[ jsonCurrentIndex++ ] = json_data[ i ];
  }
  json_string[ jsonCurrentIndex ] = '\0';
  for( u32 i = json_string_length + 1; i > json_string_length - 10; i-- ) {
    if( json_string[ i ] == 0x7D || json_string[ i ] == 0x5D )//   } or ]
      break;
    json_string[ i ] = '\0'; // make sure there's no erroneous padding bits at the end of the json, set to \0 if so
  }
}

u32 count_meshes( const char* json_string, u32 char_count ) {
  u32     result            = 0;
  bool32  in_meshes_section = false;
  u32     object_depth      = 0;
  u32     array_depth       = 0;
  char    this_char;
  char    these_chars[ 11 ];
  for( u32 i = 0; i < char_count; i++ ) {
    this_char = json_string[ i ];
    if( in_meshes_section && this_char == ASCII_CLOSE_SQUARE_BRACKETS && array_depth == 1 ) // if we've hit the end of the array
      return result;
    if( this_char == ASCII_OPEN_SQUARE_BRACKETS ) {
      array_depth++;
    } else if( this_char == ASCII_CLOSE_SQUARE_BRACKETS ) {
      array_depth--;
    } else if( this_char == ASCII_OPEN_CURLY_BRACKET ) {
      object_depth++;
    } else if( this_char == ASCII_CLOSE_CURLY_BRACKET ) {
      object_depth--;
    }
    if( object_depth == 1 && this_char == ASCII_DOUBLE_QUOTE && !in_meshes_section ) {
      for( u32 j = 0; j < 10; j++ ) {
        these_chars[ j ] = json_string[ ( i + j ) ];
      }
      these_chars[ 10 ] = '\0';
      //                                  "meshes":[
      if( strings_are_equal( these_chars, "\"meshes\":[" ) )
        in_meshes_section = true;
    }
    if( in_meshes_section && this_char == ASCII_OPEN_CURLY_BRACKET && array_depth == 1 )
      result++;
  }
  return result;
}

inline u32 get_element_type( char testChar, bool32 in_string ) {
  if( testChar == '\0' )
    return JSON_ELEMENT_BRACKET;
  if( testChar == 34 )
    return JSON_ELEMENT_DOUBLE_QUOTE;
  if( in_string || ( testChar >= 65 && testChar <= 90 ) || ( testChar >= 97 && testChar <= 122 ) )
    return JSON_ELEMENT_CHAR;
  if( testChar == 44 )
    return JSON_ELEMENT_COMMA;
  if( testChar >= 48 && testChar <= 57 )
    return JSON_ELEMENT_NUMBER;
  if( testChar == 58 )
    return JSON_ELEMENT_COLON;
  if( testChar == 91 || testChar == 93 || testChar == 123 || testChar == 125 )
    return JSON_ELEMENT_BRACKET;
  return JSON_ELEMENT_NONE;
}

void populate_mesh_name( u32 target_mesh_index, const char* json_string, u32 json_char_count, char* mesh_name ) {
  
  bool32  in_nodes_section    = false;
  u32     object_depth        = 0;
  u32     array_depth         = 0;
  s32     current_mesh_index  = -1;
  bool32  in_string           = false;
  bool32  is_end_of_value     = false;
  bool32  is_key              = true;
  u32     this_element_type   = JSON_ELEMENT_NONE;
  u32     next_element_type   = JSON_ELEMENT_NONE;
  u32     this_state          = JSON_STATE_NONE;
  u32     last_state          = JSON_STATE_NONE;
  u32     current_index       = 0;
  
  char    this_char;
  char    next_char;
  
  const u32 buffer_size = 100;
  char key  [ buffer_size ];
  char value[ buffer_size ];
  
  char these_chars[ 10 ];
  
  for( u32 i = 0; i < json_char_count; i++ ) {
    
    this_char = json_string[ i ];
    
    if( this_char == ASCII_OPEN_SQUARE_BRACKETS ) {
      array_depth++;
    } else if( this_char == ASCII_CLOSE_SQUARE_BRACKETS ) {
      array_depth--;
    } else if( this_char == ASCII_OPEN_CURLY_BRACKET ) {
      object_depth++;
    } else if( this_char == ASCII_CLOSE_CURLY_BRACKET ) {
      object_depth--;
    } else if( this_char == ASCII_DOUBLE_QUOTE ) {
      in_string = !in_string;
    }
    
    if( object_depth == 1 && this_char == ASCII_DOUBLE_QUOTE && !in_nodes_section ) {
      for( u32 j = 0; j < 9; j++ ) {
        these_chars[ j ] = json_string[ ( i + j ) ];
      }
      these_chars[ 9 ] = '\0';
      //                                  "nodes":[
      if( strings_are_equal( these_chars, "\"nodes\":[" ) )
        in_nodes_section = true;
    }
      
    if( in_nodes_section ) {
      
      
      if( this_char == ASCII_OPEN_CURLY_BRACKET )
        current_mesh_index++;
      
      if( current_mesh_index == target_mesh_index ) {
        
        if( i == json_char_count ) {
          next_char = '\0';
          next_element_type = JSON_ELEMENT_BRACKET;
        } else {
          next_char = json_string[ ( i + 1 ) ];
          next_element_type = get_element_type( next_char, in_string );
        }
        
        this_element_type = get_element_type( this_char, in_string );
        
        if( this_element_type == JSON_ELEMENT_BRACKET ) {
          this_state = JSON_STATE_READING_JSON;
        } else if( this_char == ASCII_COMMA ) {
          this_state = JSON_STATE_READING_JSON;
        } else if( this_char == ASCII_COLON ) {
          if( next_element_type == JSON_ELEMENT_DOUBLE_QUOTE || next_element_type == JSON_ELEMENT_NUMBER ) {
            this_state = JSON_STATE_READING_VALUE;
          } else {
            this_state = JSON_STATE_READING_JSON;
          }
        } else if( this_char == ASCII_DOUBLE_QUOTE && last_state == JSON_STATE_READING_JSON ) {
          this_state = JSON_STATE_READING_KEY;
          current_index = 0;
        }
        
        if( last_state != JSON_STATE_READING_KEY && this_state == JSON_STATE_READING_KEY ) {
          for( u32 j = 0; j < buffer_size; j++ ) {
            key[ j ] = '\0';
          }
        }
        
        if( last_state != JSON_STATE_READING_VALUE && this_state == JSON_STATE_READING_VALUE ) {
          for( u32 j = 0; j < buffer_size; j++ ) {
            value[ j ] = '\0';
          }
        }
        
        if( last_state == JSON_STATE_READING_VALUE && this_state != JSON_STATE_READING_VALUE ) {
          value[ current_index++ ] = '\0';
          current_index = 0;
        }
        
        if( this_state == JSON_STATE_READING_KEY && this_element_type != JSON_ELEMENT_DOUBLE_QUOTE ) {
          key[ current_index++ ] = this_char;
        }
        
        if( this_state == JSON_STATE_READING_VALUE && this_element_type != JSON_ELEMENT_DOUBLE_QUOTE ) {
          value[ current_index++ ] = this_char;
        }
        
        if( last_state == JSON_STATE_READING_KEY && this_state != JSON_STATE_READING_KEY && current_index > 0 ) {
          key[ current_index++ ] = '\0';
          current_index = 0;
        }
        
        if( last_state == JSON_STATE_READING_VALUE && this_state != JSON_STATE_READING_VALUE ) {
          // do stuff with key value pair
          // std::cout << key << " : " << value << std::endl;
          
          if( strings_are_equal( key, "name" ) ) {
            u32 value_length = string_length( value );
            for( u32 j = 0; j < value_length; j++ ) {
              mesh_name[ j ] = value[ j ];
            }
          }
        }
        last_state = this_state;
      }
    }
  }
}

s32 get_image_buffer_view_index( const char* json_string, u32 json_char_count ) {
  
  s32     result = -1;
  
  bool32  in_images_section   = false;
  u32     object_depth        = 0;
  u32     array_depth         = 0;
  s32     current_mesh_index  = -1;
  bool32  in_string           = false;
  bool32  is_end_of_value     = false;
  bool32  is_key              = true;
  u32     this_element_type   = JSON_ELEMENT_NONE;
  u32     next_element_type   = JSON_ELEMENT_NONE;
  u32     this_state          = JSON_STATE_NONE;
  u32     last_state          = JSON_STATE_NONE;
  u32     current_index       = 0;
  
  bool32  done_vertex         = false;
  bool32  done_indices        = false;
  bool32  done_normals        = false;
  bool32  done_texcoord_0     = false;
  bool32  done_material       = false;
  
  char this_char;
  char next_char;
  
  const u32 buffer_size = 100;
  char key  [ buffer_size ];
  char value[ buffer_size ];
  
  char these_chars[ 11 ];
  
  for( u32 i = 0; i < json_char_count; i++ ) {
    
    this_char = json_string[ i ];
    
    if( this_char == ASCII_OPEN_SQUARE_BRACKETS ) {
      array_depth++;
    } else if( this_char == ASCII_CLOSE_SQUARE_BRACKETS ) {
      array_depth--;
    } else if( this_char == ASCII_OPEN_CURLY_BRACKET ) {
      object_depth++;
    } else if( this_char == ASCII_CLOSE_CURLY_BRACKET ) {
      object_depth--;
    } else if( this_char == ASCII_DOUBLE_QUOTE ) {
      in_string = !in_string;
    }
    
    if( object_depth == 1 && this_char == ASCII_DOUBLE_QUOTE && !in_images_section ) {
      for( u32 j = 0; j < 10; j++ ) {
        these_chars[ j ] = json_string[ ( i + j ) ];
      }
      these_chars[ 10 ] = '\0';
      //                                  "images":[
      if( strings_are_equal( these_chars, "\"images\":[" ) )
        in_images_section = true;
      
      SDL_LogDebug( SDL_LOG_PRIORITY_DEBUG, "found %s\n", these_chars );
    }
      
    if( in_images_section ) {
      
      if( this_char == ASCII_OPEN_CURLY_BRACKET && array_depth == 1 )
        current_mesh_index++;
      
      if( 1 ) { // copy n paste, don't need an if here but leaving in for consistency
        
        if( i == json_char_count ) {
          next_char = '\0';
          next_element_type = JSON_ELEMENT_BRACKET;
        } else {
          next_char = json_string[ ( i + 1 ) ];
          next_element_type = get_element_type( next_char, in_string );
        }
        
        this_element_type = get_element_type( this_char, in_string );
        
        if( this_element_type == JSON_ELEMENT_BRACKET ) {
          this_state = JSON_STATE_READING_JSON;
        } else if( this_char == ASCII_COMMA ) {
          this_state = JSON_STATE_READING_JSON;
        } else if( this_char == ASCII_COLON ) {
          if( next_element_type == JSON_ELEMENT_DOUBLE_QUOTE || next_element_type == JSON_ELEMENT_NUMBER ) {
            this_state = JSON_STATE_READING_VALUE;
          } else {
            this_state = JSON_STATE_READING_JSON;
          }
        } else if( this_char == ASCII_DOUBLE_QUOTE && last_state == JSON_STATE_READING_JSON ) {
          this_state = JSON_STATE_READING_KEY;
          current_index = 0;
        }
        
        if( last_state != JSON_STATE_READING_KEY && this_state == JSON_STATE_READING_KEY ) {
          for( u32 j = 0; j < buffer_size; j++ ) {
            key[ j ] = '\0';
          }
        }
        
        if( last_state != JSON_STATE_READING_VALUE && this_state == JSON_STATE_READING_VALUE ) {
          for( u32 j = 0; j < buffer_size; j++ ) {
            value[ j ] = '\0';
          }
        }
        
        if( last_state == JSON_STATE_READING_VALUE && this_state != JSON_STATE_READING_VALUE ) {
          value[ current_index++ ] = '\0';
          current_index = 0;
        }
        
        if( this_state == JSON_STATE_READING_KEY && this_element_type != JSON_ELEMENT_DOUBLE_QUOTE ) {
          key[ current_index++ ] = this_char;
        }
        
        if( this_state == JSON_STATE_READING_VALUE && this_element_type != JSON_ELEMENT_DOUBLE_QUOTE ) {
          value[ current_index++ ] = this_char;
        }
        
        if( last_state == JSON_STATE_READING_KEY && this_state != JSON_STATE_READING_KEY && current_index > 0 ) {
          key[ current_index++ ] = '\0';
          current_index = 0;
        }
        
        if( last_state == JSON_STATE_READING_VALUE && this_state != JSON_STATE_READING_VALUE ) {
          // do stuff with key value pair
          // std::cout << key << " : " << value << std::endl;
          
          if( strings_are_equal( key, "bufferView" ) ) {
            result = atoi( value );
            return result;
          }
        }
        last_state = this_state;
      }
    }
  }
  
  return result;
}

MeshPositionIndices get_mesh_position_indices( u32 target_mesh_index, const char* json_string, u32 json_char_count ) {
  
  MeshPositionIndices result;
  
  bool32  in_meshes_section   = false;
  u32     object_depth        = 0;
  u32     array_depth         = 0;
  s32     current_mesh_index  = -1;
  bool32  in_string           = false;
  bool32  is_end_of_value     = false;
  bool32  is_key              = true;
  u32     this_element_type   = JSON_ELEMENT_NONE;
  u32     next_element_type   = JSON_ELEMENT_NONE;
  u32     this_state          = JSON_STATE_NONE;
  u32     last_state          = JSON_STATE_NONE;
  u32     current_index       = 0;
  
  bool32  done_vertex         = false;
  bool32  done_indices        = false;
  bool32  done_normals        = false;
  bool32  done_texcoord_0     = false;
  bool32  done_material       = false;
  
  char this_char;
  char next_char;
  
  const u32 buffer_size = 100;
  char key[ buffer_size ];
  char value[ buffer_size ];
  
  char these_chars[ 11 ];
  
  for( u32 i = 0; i < json_char_count; i++ ) {
    
    this_char = json_string[ i ];
    
    if( this_char == ASCII_OPEN_SQUARE_BRACKETS ) {
      array_depth++;
    } else if( this_char == ASCII_CLOSE_SQUARE_BRACKETS ) {
      array_depth--;
    } else if( this_char == ASCII_OPEN_CURLY_BRACKET ) {
      object_depth++;
    } else if( this_char == ASCII_CLOSE_CURLY_BRACKET ) {
      object_depth--;
    } else if( this_char == ASCII_DOUBLE_QUOTE ) {
      in_string = !in_string;
    }
    
    if( object_depth == 1 && this_char == ASCII_DOUBLE_QUOTE && !in_meshes_section ) {
      for( u32 j = 0; j < 10; j++ ) {
        these_chars[ j ] = json_string[ ( i + j ) ];
      }
      these_chars[ 10 ] = '\0';
      //                                  "meshes":[
      if( strings_are_equal( these_chars, "\"meshes\":[" ) )
        in_meshes_section = true;
    }
      
    if( in_meshes_section ) {
      
      if( this_char == ASCII_OPEN_CURLY_BRACKET && array_depth == 1 )
        current_mesh_index++;
      
      if( current_mesh_index == target_mesh_index ) {
        
        if( i == json_char_count ) {
          next_char = '\0';
          next_element_type = JSON_ELEMENT_BRACKET;
        } else {
          next_char = json_string[ ( i + 1 ) ];
          next_element_type = get_element_type( next_char, in_string );
        }
        
        this_element_type = get_element_type( this_char, in_string );
        
        // have reached the end of the array so bail
        if( in_meshes_section && array_depth == 1 && this_char == ASCII_CLOSE_SQUARE_BRACKETS )
          return result;
        
        if( this_element_type == JSON_ELEMENT_BRACKET ) {
          this_state = JSON_STATE_READING_JSON;
        } else if( this_char == ASCII_COMMA ) {
          this_state = JSON_STATE_READING_JSON;
        } else if( this_char == ASCII_COLON ) {
          if( next_element_type == JSON_ELEMENT_DOUBLE_QUOTE || next_element_type == JSON_ELEMENT_NUMBER ) {
            this_state = JSON_STATE_READING_VALUE;
          } else {
            this_state = JSON_STATE_READING_JSON;
          }
        } else if( this_char == ASCII_DOUBLE_QUOTE && last_state == JSON_STATE_READING_JSON ) {
          this_state = JSON_STATE_READING_KEY;
          current_index = 0;
        }
        
        if( last_state != JSON_STATE_READING_KEY && this_state == JSON_STATE_READING_KEY ) {
          for( u32 j = 0; j < buffer_size; j++ ) {
            key[ j ] = '\0';
          }
        }
        
        if( last_state != JSON_STATE_READING_VALUE && this_state == JSON_STATE_READING_VALUE ) {
          for( u32 j = 0; j < buffer_size; j++ ) {
            value[ j ] = '\0';
          }
        }
        
        if( last_state == JSON_STATE_READING_VALUE && this_state != JSON_STATE_READING_VALUE ) {
          value[ current_index++ ] = '\0';
          current_index = 0;
        }
        
        if( this_state == JSON_STATE_READING_KEY && this_element_type != JSON_ELEMENT_DOUBLE_QUOTE ) {
          key[ current_index++ ] = this_char;
        }
        
        if( this_state == JSON_STATE_READING_VALUE && this_element_type != JSON_ELEMENT_DOUBLE_QUOTE ) {
          value[ current_index++ ] = this_char;
        }
        
        if( last_state == JSON_STATE_READING_KEY && this_state != JSON_STATE_READING_KEY && current_index > 0 ) {
          key[ current_index++ ] = '\0';
          current_index = 0;
        }
        
        if( last_state == JSON_STATE_READING_VALUE && this_state != JSON_STATE_READING_VALUE ) {
          // do stuff with key value pair
          // std::cout << key << " : " << value << std::endl;
          
          if( strings_are_equal( key, "POSITION" ) ) {
            result.vertices = atoi( value );
            done_vertex = true;
          } else if( strings_are_equal( key, "NORMAL" ) ) {
            result.normals = atoi( value );
            done_normals = true;
          } else if( strings_are_equal( key, "TEXCOORD_0" ) ) {
            result.texcoord_0 = atoi( value );
            done_texcoord_0 = true;
          } else if( strings_are_equal( key, "indices" ) ) {
            result.indices = atoi( value );
            done_indices = true;
          } else if( strings_are_equal( key, "material" ) ) {
            result.material = atoi( value );
            done_material = true;
          }
          
          if( done_vertex && done_indices && done_normals && done_texcoord_0 && done_material )
            return result;
        }
        last_state = this_state;
      }
    }
  }
  
  return result;
}

AccessorData get_accessor_data( u32 target_accessor_index, const char* json_string, u32 json_char_count ) {
  
  AccessorData result = {};
  
  bool32  in_accessors_section    = false;
  u32     object_depth            = 0;
  u32     array_depth             = 0;
  s32     current_accessor_index  = -1;
  bool32  in_string               = false;
  bool32  is_end_of_value         = false;
  bool32  is_key                  = true;
  u32     this_element_type       = JSON_ELEMENT_NONE;
  u32     next_element_type       = JSON_ELEMENT_NONE;
  u32     this_state              = JSON_STATE_NONE;
  u32     last_state              = JSON_STATE_NONE;
  u32     current_index           = 0;
  
  bool32  done_buffer_view        = false;
  bool32  done_component_type     = false;
  bool32  done_count              = false;
  bool32  done_type               = false;
  
  char this_char;
  char next_char;
  
  const u32 buffer_size = 100;
  char key  [ buffer_size ];
  char value[ buffer_size ];
  
  char these_chars[ 14 ];
  
  for( u32 i = 0; i < json_char_count; i++ ) {
    
    this_char = json_string[ i ];
        // debug - delete for prod
        if( i == json_char_count ) {
          next_char = '\0';
          next_element_type = JSON_ELEMENT_BRACKET;
        } else {
          next_char = json_string[ ( i + 1 ) ];
          next_element_type = get_element_type( next_char, in_string );
        }
    
    
    if( this_char == ASCII_OPEN_SQUARE_BRACKETS ) {
      array_depth++;
    } else if( this_char == ASCII_CLOSE_SQUARE_BRACKETS ) {
      array_depth--;
    } else if( this_char == ASCII_OPEN_CURLY_BRACKET ) {
      object_depth++;
    } else if( this_char == ASCII_CLOSE_CURLY_BRACKET ) {
      object_depth--;
    } else if( this_char == ASCII_DOUBLE_QUOTE ) {
      in_string = !in_string;
    }
    
    if( object_depth == 1 && this_char == ASCII_DOUBLE_QUOTE && !in_accessors_section ) {
      for( u32 j = 0; j < 13; j++ ) {
        these_chars[ j ] = json_string[ ( i + j ) ];
      }
      these_chars[ 13 ] = '\0';
      //                                  "accessors":[
      if( strings_are_equal( these_chars, "\"accessors\":[" ) ) {
        in_accessors_section = true;
      }
    }
      
    if( in_accessors_section ) {
      
      if( this_char == ASCII_OPEN_CURLY_BRACKET && array_depth == 1 )
        current_accessor_index++;
      
      if( current_accessor_index == target_accessor_index ) {
        
        if( i == json_char_count ) {
          next_char = '\0';
          next_element_type = JSON_ELEMENT_BRACKET;
        } else {
          next_char = json_string[ ( i + 1 ) ];
          next_element_type = get_element_type( next_char, in_string );
        }
        
        this_element_type = get_element_type( this_char, in_string );
        
        if( this_element_type == JSON_ELEMENT_BRACKET ) {
          this_state = JSON_STATE_READING_JSON;
        } else if( this_char == ASCII_COMMA ) {
          this_state = JSON_STATE_READING_JSON;
        } else if( this_char == ASCII_COLON ) {
          if( next_element_type == JSON_ELEMENT_DOUBLE_QUOTE || next_element_type == JSON_ELEMENT_NUMBER ) {
            this_state = JSON_STATE_READING_VALUE;
          } else {
            this_state = JSON_STATE_READING_JSON;
          }
        } else if( this_char == ASCII_DOUBLE_QUOTE && last_state == JSON_STATE_READING_JSON ) {
          this_state = JSON_STATE_READING_KEY;
          current_index = 0;
        }
        
        if( last_state != JSON_STATE_READING_KEY && this_state == JSON_STATE_READING_KEY ) {
          for( u32 j = 0; j < buffer_size; j++ ) {
            key[ j ] = '\0';
          }
        }
        
        if( last_state != JSON_STATE_READING_VALUE && this_state == JSON_STATE_READING_VALUE ) {
          for( u32 j = 0; j < buffer_size; j++ ) {
            value[ j ] = '\0';
          }
        }
        
        if( last_state == JSON_STATE_READING_VALUE && this_state != JSON_STATE_READING_VALUE ) {
          value[ current_index++ ] = '\0';
          current_index = 0;
        }
        
        if( this_state == JSON_STATE_READING_KEY && this_element_type != JSON_ELEMENT_DOUBLE_QUOTE ) {
          key[ current_index++ ] = this_char;
        }
        
        if( this_state == JSON_STATE_READING_VALUE && this_element_type != JSON_ELEMENT_DOUBLE_QUOTE ) {
          value[ current_index++ ] = this_char;
        }
        
        if( last_state == JSON_STATE_READING_KEY && this_state != JSON_STATE_READING_KEY && current_index > 0 ) {
          key[ current_index++ ] = '\0';
          current_index = 0;
        }
        
        if( last_state == JSON_STATE_READING_VALUE && this_state != JSON_STATE_READING_VALUE ) {
          // do stuff with key value pair
          // std::cout << key << " : " << value << std::endl;
          
          if( strings_are_equal( key, "bufferView" ) ) {
            result.buffer_view = atoi( value );
            done_buffer_view = true;
          } else if( strings_are_equal( key, "componentType" ) ) {
            result.component_type = atoi( value );
            done_component_type = true;
          } else if( strings_are_equal( key, "count" ) ) {
            result.count = atoi( value );
            done_count = true;
          } else if( strings_are_equal( key, "type" ) ) {
            
            u32 value_length = string_length( value );
            
            for( u32 j = 0; j < 6; j++ ) {
              if( j < value_length ) {
                result.type[ j ] = value[ j ];
              } else {
                result.type[ j ] = '\0';
              }
            }
            
            done_type = true;
          }
          
          if( done_buffer_view && done_component_type && done_count && done_type )
            return result;
        }
        last_state = this_state;
      }
    }
  }
  return result;
}

BufferViewData get_buffer_view_data( u32 target_buffer_view_index, const char* json_string, u32 json_char_count ) {
  
  BufferViewData result;
  
  bool32  in_buffer_views_section   = false;
  u32     object_depth              = 0;
  u32     array_depth               = 0;
  s32     current_buffer_view_index = -1;
  bool32  in_string                 = false;
  bool32  is_end_of_value           = false;
  bool32  is_key                    = true;
  u32     this_element_type         = JSON_ELEMENT_NONE;
  u32     next_element_type         = JSON_ELEMENT_NONE;
  u32     this_state                = JSON_STATE_NONE;
  u32     last_state                = JSON_STATE_NONE;
  u32     current_index             = 0;
  
  bool32  done_buffer               = false;
  bool32  done_byte_length          = false;
  bool32  done_byte_offset          = false;
  
  char this_char;
  char next_char;
  
  const u32 buffer_size = 100;
  char key  [ buffer_size ];
  char value[ buffer_size ];
  
  char these_chars[ 16 ];
  
  // "bufferViews":[
  
  for( u32 i = 0; i < json_char_count; i++ ) {
    
    this_char = json_string[ i ];
    
    if( this_char == ASCII_OPEN_SQUARE_BRACKETS ) {
      array_depth++;
    } else if( this_char == ASCII_CLOSE_SQUARE_BRACKETS ) {
      array_depth--;
    } else if( this_char == ASCII_OPEN_CURLY_BRACKET ) {
      object_depth++;
    } else if( this_char == ASCII_CLOSE_CURLY_BRACKET ) {
      object_depth--;
    } else if( this_char == ASCII_DOUBLE_QUOTE ) {
      in_string = !in_string;
    }
    
    if( object_depth == 1 && this_char == ASCII_DOUBLE_QUOTE && !in_buffer_views_section ) {
      for( u32 j = 0; j < 15; j++ ) {
        these_chars[ j ] = json_string[ ( i + j ) ];
      }
      these_chars[ 15 ] = '\0';
      //                                  "meshes":[
      if( strings_are_equal( these_chars, "\"bufferViews\":[" ) )
        in_buffer_views_section = true;
      
    }
    
    if( in_buffer_views_section ) {
      if( this_char == ASCII_OPEN_CURLY_BRACKET && array_depth == 1 )
        current_buffer_view_index++;
      
      if( current_buffer_view_index == target_buffer_view_index ) {
        if( i == json_char_count ) {
          next_char = '\0';
          next_element_type = JSON_ELEMENT_BRACKET;
        } else {
          next_char = json_string[ ( i + 1 ) ];
          next_element_type = get_element_type( next_char, in_string );
        }
        
        this_element_type = get_element_type( this_char, in_string );
        
        if( this_element_type == JSON_ELEMENT_BRACKET ) {
          this_state = JSON_STATE_READING_JSON;
        } else if( this_char == ASCII_COMMA ) {
          this_state = JSON_STATE_READING_JSON;
        } else if( this_char == ASCII_COLON ) {
          if( next_element_type == JSON_ELEMENT_DOUBLE_QUOTE || next_element_type == JSON_ELEMENT_NUMBER ) {
            this_state = JSON_STATE_READING_VALUE;
          } else {
            this_state = JSON_STATE_READING_JSON;
          }
        } else if( this_char == ASCII_DOUBLE_QUOTE && last_state == JSON_STATE_READING_JSON ) {
          this_state = JSON_STATE_READING_KEY;
          current_index = 0;
        }
        
        if( last_state != JSON_STATE_READING_KEY && this_state == JSON_STATE_READING_KEY ) {
          for( u32 j = 0; j < buffer_size; j++ ) {
            key[ j ] = '\0';
          }
        }
        
        if( last_state != JSON_STATE_READING_VALUE && this_state == JSON_STATE_READING_VALUE ) {
          for( u32 j = 0; j < buffer_size; j++ ) {
            value[ j ] = '\0';
          }
        }
        
        if( last_state == JSON_STATE_READING_VALUE && this_state != JSON_STATE_READING_VALUE ) {
          value[ current_index++ ] = '\0';
          current_index = 0;
        }
        
        if( this_state == JSON_STATE_READING_KEY && this_element_type != JSON_ELEMENT_DOUBLE_QUOTE ) {
          key[ current_index++ ] = this_char;
        }
        
        if( this_state == JSON_STATE_READING_VALUE && this_element_type != JSON_ELEMENT_DOUBLE_QUOTE ) {
          value[ current_index++ ] = this_char;
        }
        
        if( last_state == JSON_STATE_READING_KEY && this_state != JSON_STATE_READING_KEY && current_index > 0 ) {
          key[ current_index++ ] = '\0';
          current_index = 0;
        }
        
        if( last_state == JSON_STATE_READING_VALUE && this_state != JSON_STATE_READING_VALUE ) {
          // do stuff with key value pair
          // std::cout << key << " : " << value << std::endl;
          
          if( strings_are_equal( key, "buffer" ) ) {
            result.buffer = atoi( value );
            done_buffer = true;
          } else if( strings_are_equal( key, "byteLength" ) ) {
            result.byte_length = atoi( value );
            done_byte_length = true;
          } else if( strings_are_equal( key, "byteOffset" ) ) {
            result.byte_offset = atoi( value );
            done_byte_offset = true;
          }
          
          if( done_buffer && done_byte_length && done_byte_offset )
            return result;
        }
        last_state = this_state;
      }
    }
  }
  return result;
}

MeshData populate_mesh_data( u32 target_mesh_index, const char* json_string, ReadFileResult* gltf_file, f32 scale ) {
  
  MeshData result;
  
  const char* gltf_contents = ( char* )gltf_file -> contents;
  u32         filesize      = gltf_file -> contents_size;
  GltfHeader* gltf_header   = ( GltfHeader* ) gltf_contents; // pulls in fixed length json stuff too
  
  u32  gltf_header_size_in_bytes = 12; // magic + version + length
  u32  json_header_in_bytes      = 8; // 4 bytes for chunk length, 4 bytes for chunk type
  u32  bin_header_in_bytes       = 8; // 4 bytes for chunk length, 4 bytes for chunk type
  u32  json_string_in_bytes      = gltf_header -> json_chunk_length;
  u32  json_char_count           = string_length( json_string ); // without padding
  u32* bin_chunk_length          = ( u32* )( ( u8* )gltf_contents + gltf_header_size_in_bytes + json_header_in_bytes + json_string_in_bytes );
  u32  bin_size_in_bytes         = *bin_chunk_length;
  u32  bin_start_offset          = gltf_header_size_in_bytes + json_header_in_bytes + json_string_in_bytes + bin_header_in_bytes;
  u32  json_chunk_in_bytes       = bin_header_in_bytes + json_string_in_bytes;
  
  const u32 char_buffer_size = 100;
  char mesh_name[ char_buffer_size ];
  for( u32 i = 0; i < char_buffer_size; i++ ) {
    mesh_name[ i ] = '\0';
  }
  populate_mesh_name( target_mesh_index, json_string, json_char_count, mesh_name );
  
  if( strings_are_equal( mesh_name, "Collider" ) ) {
    result.is_collider = true;
  }
  
  MeshPositionIndices mesh_position_indices = get_mesh_position_indices( target_mesh_index, json_string, json_char_count );
  
  AccessorData vertex_accessor_data     = get_accessor_data( mesh_position_indices.vertices, json_string, json_char_count );
  AccessorData normal_accessor_data     = get_accessor_data( mesh_position_indices.normals, json_string, json_char_count );
  AccessorData index_accessor_data      = get_accessor_data( mesh_position_indices.indices, json_string, json_char_count );
  AccessorData tex_coord0_accessor_data = get_accessor_data( mesh_position_indices.texcoord_0, json_string, json_char_count );
  
  BufferViewData vertex_buffer_view_data      = get_buffer_view_data( vertex_accessor_data.buffer_view, json_string, json_char_count );
  BufferViewData normal_buffer_view_data      = get_buffer_view_data( normal_accessor_data.buffer_view, json_string, json_char_count );
  BufferViewData index_buffer_view_data       = get_buffer_view_data( index_accessor_data.buffer_view, json_string, json_char_count );
  BufferViewData tex_coord0_buffer_view_data  = get_buffer_view_data( tex_coord0_accessor_data.buffer_view, json_string, json_char_count );
  
  u32 vertex_data_total_offset     = bin_start_offset + vertex_buffer_view_data.byte_offset;
  u32 normal_data_total_offset     = bin_start_offset + normal_buffer_view_data.byte_offset;
  u32 index_data_total_offset      = bin_start_offset + index_buffer_view_data.byte_offset;
  u32 tex_coord0_data_total_offset = bin_start_offset + tex_coord0_buffer_view_data.byte_offset;
  
  result.vertex_data_in_bytes     = vertex_buffer_view_data.byte_length;
  result.normal_data_in_bytes     = normal_buffer_view_data.byte_length;
  result.index_data_raw_in_bytes  = index_buffer_view_data.byte_length;
  result.tex_coord0_data_in_bytes = tex_coord0_buffer_view_data.byte_length;
  result.vertex_count             = vertex_accessor_data.count;
  result.normal_count             = normal_accessor_data.count;
  result.index_count              = index_accessor_data.count;
  result.tex_coord0_count         = tex_coord0_accessor_data.count;
  result.vertex_data              = ( f32* )( ( char* )gltf_contents + vertex_data_total_offset );
  result.normal_data              = ( f32* )( ( char* )gltf_contents + normal_data_total_offset );
  result.tex_coord0_data          = ( f32* )( ( char* )gltf_contents + tex_coord0_data_total_offset );
  result.index_data_raw           = ( u16* )( ( char* )gltf_contents + index_data_total_offset );
  
  s32 image_buffer_view_index = get_image_buffer_view_index( json_string, json_char_count );
  if( image_buffer_view_index >= 0  ) {
    BufferViewData image_buffer_view_data = get_buffer_view_data( image_buffer_view_index, json_string, json_char_count );
    result.image_data_in_bytes            = image_buffer_view_data.byte_length;
    u32 image_data_total_offset        = bin_start_offset + image_buffer_view_data.byte_offset;
    result.image_data                     = ( u8* )( ( char* )gltf_contents + image_data_total_offset );
  }
  
  if( scale != 1.0f ) {
    f32* value = result.vertex_data;
    
    for( u32 i = 0; i < result.vertex_count; i++ ) {
      f32 this_float = *value;
      
      this_float *= scale;
      *value = this_float;
      
      value++;
    }
  }
  
  return result;
}

GltfbufferViewInfo get_glft_buffer_view_info( u32 target_mesh_index, const char* gltf_contents, JsonString json, s32 content_type ) {
  
  GltfbufferViewInfo result = {};
  
  char*   json_string     = json.json_string;
  u32  json_char_count = json.json_char_count;
  
  MeshPositionIndices mesh_position_indices = get_mesh_position_indices( target_mesh_index, json_string, json_char_count );
  AccessorData        accessor_data;
  
  if( content_type == GLTF_VERTICES ) {
    accessor_data = get_accessor_data( mesh_position_indices.vertices, json_string, json_char_count );
  } else if( content_type == GLTF_INDICES ) {
    accessor_data = get_accessor_data( mesh_position_indices.indices, json_string, json_char_count );
  } else if( content_type == GLTF_NORMALS ) {
    accessor_data = get_accessor_data( mesh_position_indices.normals, json_string, json_char_count );
  } else if( content_type == GLTF_TEX_COORD0 ) {
    accessor_data = get_accessor_data( mesh_position_indices.texcoord_0, json_string, json_char_count );
  } else {
    return result;
  }
  
  BufferViewData buffer_view_data = get_buffer_view_data( accessor_data.buffer_view, json_string, json_char_count );
  
  result.buffer_view    = accessor_data.buffer_view;
  result.component_type = accessor_data.component_type;
  result.count          = accessor_data.count;
  result.type           = accessor_data.accessor_type;
  result.buffer         = buffer_view_data.buffer;
  result.byte_length    = buffer_view_data.byte_length;
  result.byte_offset    = buffer_view_data.byte_offset;
  
  return result;
}

void* get_gltf_data_pointer( u32 target_mesh_index, const char* gltf_contents, JsonString json, s32 content_type ) {
  
  void* result = NULL;
  
  char*       json_string     = json.json_string;
  u32         json_char_count = json.json_char_count;
  GltfHeader* gltf_header     = ( GltfHeader* ) gltf_contents; // pulls in fixed length json stuff too
  
  MeshPositionIndices mesh_position_indices = get_mesh_position_indices( target_mesh_index, json_string, json_char_count );
  AccessorData        accessor_data;
  u32                 buffer_view;
  
  if( content_type == GLTF_VERTICES ) {
    accessor_data = get_accessor_data( mesh_position_indices.vertices, json_string, json_char_count );
  } else if( content_type == GLTF_INDICES ) {
    accessor_data = get_accessor_data( mesh_position_indices.indices, json_string, json_char_count );
  } else if( content_type == GLTF_NORMALS ) {
    accessor_data = get_accessor_data( mesh_position_indices.normals, json_string, json_char_count );
  } else if( content_type == GLTF_TEX_COORD0 ) {
    accessor_data = get_accessor_data( mesh_position_indices.texcoord_0, json_string, json_char_count );
  } else if( content_type == GLTF_IMAGE ) {
    buffer_view = get_image_buffer_view_index( json.json_string, json.json_char_count );
  }
  
  if( content_type == GLTF_IMAGE ) {
    buffer_view = get_image_buffer_view_index( json.json_string, json.json_char_count );
  } else {
    buffer_view = accessor_data.buffer_view;
  }
  
  BufferViewData  buffer_view_data  = get_buffer_view_data( buffer_view, json_string, json_char_count );
  u32             bin_start_offset  = get_bin_start_offset( gltf_header );
  u32             total_offset      = bin_start_offset + buffer_view_data.byte_offset;
  
  result = ( void* )( ( char* )gltf_contents + total_offset );
  
  return result;
}

u32 get_gltf_data_count( u32 target_mesh_index, const char* gltf_contents, JsonString json, s32 content_type ) {
  
  u32 result = 0;
  
  char*   json_string     = json.json_string;
  u32  json_char_count = json.json_char_count;
  u32  count_per_value = 0;
  
  MeshPositionIndices mesh_position_indices = get_mesh_position_indices( target_mesh_index, json_string, json_char_count );
  AccessorData        accessor_data;
  
  if( content_type == GLTF_VERTICES ) {
    accessor_data = get_accessor_data( mesh_position_indices.vertices, json_string, json_char_count );
    count_per_value = 3;
  } else if( content_type == GLTF_INDICES ) {
    accessor_data = get_accessor_data( mesh_position_indices.indices, json_string, json_char_count );
    count_per_value = 1;
  } else if( content_type == GLTF_NORMALS ) {
    accessor_data = get_accessor_data( mesh_position_indices.normals, json_string, json_char_count );
    count_per_value = 3;
  } else if( content_type == GLTF_TEX_COORD0 ) {
    accessor_data = get_accessor_data( mesh_position_indices.texcoord_0, json_string, json_char_count );
    count_per_value = 2;
  } else {
    return result;
  }
  
  result = count_per_value * accessor_data.count;
  
  return result;
}

#endif //JSON_HPP
