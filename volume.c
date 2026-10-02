#include <pulse/volume.h>
#include <stdio.h>
#include <pulse/pulseaudio.h>

static void sink_info_callback(pa_context *context, const pa_sink_info *info, int eol, void *userdata)
{
    (void)context;

    pa_mainloop *mainloop = userdata;

    if (eol > 0) {
        pa_mainloop_quit(mainloop, 0);
        return;
    }

    if (info == NULL) {
        fprintf(stderr, "Failed to get sink information.\n");
        pa_mainloop_quit(mainloop, 1);
        return;
    }

    int volume = (pa_cvolume_avg(&info->volume) * 100 + PA_VOLUME_NORM / 2) / PA_VOLUME_NORM;

    if (info->mute) printf("Volume (MUTED): %d%%\n", volume);
    else printf("Volume: %d%%\n", volume);

    pa_mainloop_quit(mainloop, 0);
}

static void context_state_callback(pa_context *context, void *userdata)
{
    pa_mainloop *mainloop = userdata;

    switch (pa_context_get_state(context)) {
        case PA_CONTEXT_READY:
            pa_context_get_sink_info_by_name(context, "@DEFAULT_SINK@", sink_info_callback, mainloop);
            break;
        case PA_CONTEXT_FAILED:
        case PA_CONTEXT_TERMINATED:
            pa_mainloop_quit(mainloop, 1);
            break;
        default:
            break;
    }
}

int main(void)
{
    pa_mainloop *mainloop = pa_mainloop_new();
    if (mainloop == NULL) {
        fprintf(stderr, "Failed to create PulseAudio mainloop.\n");
        return 1;
    }

    pa_mainloop_api *mainloop_api = pa_mainloop_get_api(mainloop);

    pa_context *context = pa_context_new(mainloop_api, "Volume Reader");
    if (context == NULL) {
        fprintf(stderr, "Failed to create pulseaudio context.\n");
        pa_mainloop_free(mainloop);
        return 1;
    }

    pa_context_set_state_callback(context, context_state_callback, mainloop);

    if (pa_context_connect(context, NULL, PA_CONTEXT_NOFLAGS, NULL) < 0) {
        fprintf(stderr, "Failed to connect to pulseaudio.\n");

        pa_context_unref(context);
        pa_mainloop_free(mainloop);

        return 1;
    }

    int ret;
    if (pa_mainloop_run(mainloop, &ret) < 0) {
        fprintf(stderr, "Mainloop failed.\n");
    }

    pa_context_disconnect(context);
    pa_context_unref(context);
    pa_mainloop_free(mainloop);

    return ret;
}
