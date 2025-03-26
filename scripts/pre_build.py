Import("env")
import os
import shutil

def apply_patches(target, source, env):
    lib_dir = env.subst("$PROJECT_DIR/.pio/libdeps/$PIOENV/FlexCAN_T4")
    patch_dir = env.subst("$PROJECT_DIR/patches")
    
    if os.path.exists(os.path.join(lib_dir, "FlexCAN_T4.tpp")):
        os.system(f'patch {lib_dir}/FlexCAN_T4.tpp {patch_dir}/flexcan_t4_fix.patch')

env.AddPreAction("buildprog", apply_patches)