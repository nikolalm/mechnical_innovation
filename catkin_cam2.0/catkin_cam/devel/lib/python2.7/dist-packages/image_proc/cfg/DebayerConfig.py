## *********************************************************
##
## File autogenerated for the image_proc package
## by the dynamic_reconfigure package.
## Please do not edit.
##
## ********************************************************/

from dynamic_reconfigure.encoding import extract_params

inf = float('inf')

config_description = {'upper': 'DEFAULT', 'lower': 'groups', 'srcline': 245, 'name': 'Default', 'parent': 0, 'srcfile': '/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'cstate': 'true', 'parentname': 'Default', 'class': 'DEFAULT', 'field': 'default', 'state': True, 'parentclass': '', 'groups': [], 'parameters': [{'srcline': 290, 'description': 'Debayering algorithm', 'max': 3, 'cconsttype': 'const int', 'ctype': 'int', 'srcfile': '/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'name': 'debayer', 'edit_method': "{'enum_description': 'Debayering algorithm', 'enum': [{'srcline': 10, 'description': 'Fast algorithm using bilinear interpolation', 'srcfile': '/home/robot/catkin_cam/src/image_pipeline/image_proc/cfg/Debayer.cfg', 'cconsttype': 'const int', 'value': 0, 'ctype': 'int', 'type': 'int', 'name': 'Bilinear'}, {'srcline': 12, 'description': 'Edge-aware algorithm', 'srcfile': '/home/robot/catkin_cam/src/image_pipeline/image_proc/cfg/Debayer.cfg', 'cconsttype': 'const int', 'value': 1, 'ctype': 'int', 'type': 'int', 'name': 'EdgeAware'}, {'srcline': 14, 'description': 'Weighted edge-aware algorithm', 'srcfile': '/home/robot/catkin_cam/src/image_pipeline/image_proc/cfg/Debayer.cfg', 'cconsttype': 'const int', 'value': 2, 'ctype': 'int', 'type': 'int', 'name': 'EdgeAwareWeighted'}, {'srcline': 16, 'description': 'Slow but high quality Variable Number of Gradients algorithm', 'srcfile': '/home/robot/catkin_cam/src/image_pipeline/image_proc/cfg/Debayer.cfg', 'cconsttype': 'const int', 'value': 3, 'ctype': 'int', 'type': 'int', 'name': 'VNG'}]}", 'default': 0, 'level': 0, 'min': 0, 'type': 'int'}], 'type': '', 'id': 0}

min = {}
max = {}
defaults = {}
level = {}
type = {}
all_level = 0

#def extract_params(config):
#    params = []
#    params.extend(config['parameters'])
#    for group in config['groups']:
#        params.extend(extract_params(group))
#    return params

for param in extract_params(config_description):
    min[param['name']] = param['min']
    max[param['name']] = param['max']
    defaults[param['name']] = param['default']
    level[param['name']] = param['level']
    type[param['name']] = param['type']
    all_level = all_level | param['level']

Debayer_Bilinear = 0
Debayer_EdgeAware = 1
Debayer_EdgeAwareWeighted = 2
Debayer_VNG = 3
