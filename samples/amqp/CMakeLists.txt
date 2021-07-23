project(${PROJECT_NAME}-amqp
        VERSION ${PROJECT_VERSION}
        DESCRIPTION "fty messagebus sample for amqp"
        LANGUAGES CXX
)

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Pub/Sub
etn_target(exe ${PROJECT_NAME}-pub-sub PRIVATE
  SOURCES
    src/*PubSub.cpp
  INCLUDE_DIRS
    private_include
  USES_PRIVATE
    ${PROJECT_NAME_UNDERSCORE}
    ${SAMPLE_DTO_LIB_NAME}
  FLAGS
    "-fmax-errors=1"
)

# Math sample replyer
etn_target(exe ${PROJECT_NAME}-async-reply PRIVATE
  SOURCES
    src/*SampleAsyncReply.cpp
  INCLUDE_DIRS
    private_include
  USES_PUBLIC
    ${PROJECT_NAME_UNDERSCORE}
    ${SAMPLE_DTO_LIB_NAME}
  FLAGS
    "-fmax-errors=1"
)

# Math sample asynchrone and synchrone requester
etn_target(exe ${PROJECT_NAME}-send-request PRIVATE
  SOURCES
    src/*SampleSendRequest.cpp
  INCLUDE_DIRS
     private_include
  USES_PUBLIC
    ${PROJECT_NAME_UNDERSCORE}
    ${SAMPLE_DTO_LIB_NAME}
  FLAGS
    "-fmax-errors=1"
)