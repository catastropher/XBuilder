// This file is part of XBuilder.
//
// XBuilder is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// XBuilder is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with XBuilder. If not, see <http://www.gnu.org/licenses/>.

#include "X3D.h"
#include "mouse.h"

#include <alloca.h>

extern X3D_Level* global_level;

static struct {
  union {
    struct {
      int16 base_v;
      int16 creation_plane_y;
    } add_new_segment;
    
    struct {
      int16 seg_id;
      X3D_Vex3D displacement_from_center;
      _Bool selected;
    } move_segment;
  };
} command_data;


void add_segment_left_click_callback(MouseHover* hover) {
  x3d_log(X3D_INFO, "Clicked!\n");
  
  X3D_Prism3D new_seg = { .v = alloca(1000) };
  x3d_prism3d_construct(&new_seg, command_data.add_new_segment.base_v, 200, 200, (X3D_Vex3D_angle256) { 0, 0, 0 });
  x3d_prism3d_set_center(&new_seg, &hover->creation_plane_intersection);
  
  x3d_level_add_new_standalone_segment(global_level, &new_seg, 0);
}

extern int16 creation_plane_y;
extern int16 vertical_movement_plane_x;

void command_add_segment(void) {
  int base_v;
  printf("Number of vertices in base (0 to abort): ");
  scanf("%d", &base_v);
  
  command_data.add_new_segment.base_v = base_v;
  
  if(command_data.add_new_segment.base_v == 0)
    return;
  
  int y_coord;
  printf("y-coordinate of center: ");
  scanf("%d", &y_coord);
  
  command_data.add_new_segment.creation_plane_y = y_coord;
  creation_plane_y = command_data.add_new_segment.creation_plane_y;
  
  reset_mouse_callbacks();
  set_left_click_callback(add_segment_left_click_callback, X3D_FALSE);
  set_right_click_callback(NULL, X3D_FALSE);
}

void command_move_segment_left_click_callback(MouseHover* hover) {
  if(hover->type == MOUSE_HOVER_NOTHING && !command_data.move_segment.selected)
    return;
  
  X3D_LevelSegment* seg;
  
  uint16 hover_seg = x3d_segfaceid_seg(hover->segface);
  uint16 seg_id = (command_data.move_segment.selected ? command_data.move_segment.seg_id : hover_seg);
  
  seg = x3d_level_get_segmentptr(global_level, seg_id);
  X3D_Prism3D geo = { .v = alloca(1000) };
  
  x3d_levelsegment_get_geometry(global_level, seg, &geo);
  
  X3D_Vex3D center;
  x3d_prism3d_center(&geo, &center);
  
  if(!command_data.move_segment.selected) {
    creation_plane_y = center.y;
    determine_what_mouse_is_hovering_over(x3d_playermanager_get()->player[0].cam, hover, 0);
    
    hover->creation_plane_intersection.y = center.y;
    command_data.move_segment.seg_id = hover_seg;
    command_data.move_segment.selected = X3D_TRUE;
  }
  
  x3d_prism3d_set_center(&geo, &hover->creation_plane_intersection);
  x3d_levelsegment_update_geometry(global_level, seg, &geo);
}

void command_move_segment_right_click_callback(MouseHover* hover) {
  if(hover->type == MOUSE_HOVER_NOTHING && !command_data.move_segment.selected)
    return;
  
  X3D_LevelSegment* seg;
  
  uint16 hover_seg = x3d_segfaceid_seg(hover->segface);
  uint16 seg_id = (command_data.move_segment.selected ? command_data.move_segment.seg_id : hover_seg);
  
  seg = x3d_level_get_segmentptr(global_level, seg_id);
  X3D_Prism3D geo = { .v = alloca(1000) };
  
  x3d_levelsegment_get_geometry(global_level, seg, &geo);
  
  X3D_Vex3D center;
  x3d_prism3d_center(&geo, &center);
  
  if(!command_data.move_segment.selected) {
    vertical_movement_plane_x = center.x;
    determine_what_mouse_is_hovering_over(x3d_playermanager_get()->player[0].cam, hover, 0);
    
    hover->creation_plane_intersection.y = center.y;
    command_data.move_segment.seg_id = hover_seg;
    command_data.move_segment.selected = X3D_TRUE;
  }
  
  X3D_Vex3D new_center = x3d_vex3d_make(center.x, hover->vertical_plane_intersection.y, center.z);
  
  x3d_prism3d_set_center(&geo, &new_center);
  x3d_levelsegment_update_geometry(global_level, seg, &geo);
}

void command_move_segment_click_release_callback(MouseHover* hover) {
  command_data.move_segment.selected = X3D_FALSE;
}

void command_move_segment(void) {
  printf("Left click and drag to move a segment in the XZ plane\n");
  printf("Right click and drag to move a segment in the Y direction\n");
  
  command_data.move_segment.selected = X3D_FALSE;
  
  reset_mouse_callbacks();
  set_left_click_callback(command_move_segment_left_click_callback, X3D_TRUE);
  set_right_click_callback(command_move_segment_right_click_callback, X3D_TRUE);
  set_left_release_callback(command_move_segment_click_release_callback);
  set_right_release_callback(command_move_segment_click_release_callback);
}



