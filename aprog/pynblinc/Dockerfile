# Use this to create a Docker image with an entire terminal based DAW 
# Find out how to mount volumes, though, so that your productions are
# persisted on disk.

FROM debian
WORKDIR /root
RUN apt-get install -yqq tmux neovim git ranger python3 pip3 cmake; \
    apt-get build-dep csound; \
# TODO : Check out a specific commit to ensure consistency 
    git clone https://github.com/csound/csound.git; \
    cd csound; \
    # TODO : Install csound! \ 
    pip3 install ctcsound
# TODO : Clone 
COPY core/ /opt/core
COPY doc/ /opt/doc
COPY pynblinc.conf /opt/pynblinc.conf
RUN echo "export PATH=$PATH:/opt/core" >> /root/.bashrc
RUN echo "alias run='python3 /opt/core/pynblinc.py'" >> /root/.bashrc
CMD ["bash"]
