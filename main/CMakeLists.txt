# # Edit following two lines to set component requirements (see docs)
# set(COMPONENT_REQUIRES )
# set(COMPONENT_PRIV_REQUIRES )

# set(COMPONENT_SRCS "main.c" "i2c_api.c" "max30102_api.c" "algorithm.c")
# set(COMPONENT_ADD_INCLUDEDIRS "")

# register_component()

idf_component_register(SRCS "main.c" "i2c_api.c" "max30102_api.c" "algorithm.c"
                    INCLUDE_DIRS ".")
