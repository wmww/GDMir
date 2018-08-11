/*
 * Copyright © 2016-2018 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 or 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Alan Griffiths <alan@octopull.co.uk>
 */

#ifndef MIRAL_SHELL_FLOATING_WINDOW_MANAGER_H
#define MIRAL_SHELL_FLOATING_WINDOW_MANAGER_H

#include <miral/canonical_window_manager.h>

#include "splash_session.h"

#include <chrono>
#include <map>

namespace miral { class InternalClientLauncher; }

using namespace mir::geometry;

class DecorationProvider;

class FloatingWindowManagerPolicy : public miral::CanonicalWindowManagerPolicy
{
public:
    FloatingWindowManagerPolicy(
        miral::WindowManagerTools const& tools,
        std::shared_ptr<SplashSession> const& spinner,
        miral::InternalClientLauncher const& launcher,
        std::function<void()>& shutdown_hook);
    ~FloatingWindowManagerPolicy();

    virtual miral::WindowSpecification place_new_window(
        miral::ApplicationInfo const& app_info, miral::WindowSpecification const& request_parameters) override;

    /** @name example event handling:
     *  o Switch apps: Alt+Tab, tap or click on the corresponding window
     *  o Switch window: Alt+`, tap or click on the corresponding window
     *  o Move window: Alt-leftmousebutton drag (three finger drag)
     *  o Resize window: Alt-middle_button drag (three finger pinch)
     *  o Maximize/restore current window (to display size): Alt-F11
     *  o Maximize/restore current window (to display height): Shift-F11
     *  o Maximize/restore current window (to display width): Ctrl-F11
     *  o Switch workspace . . . . . . . . . . : Meta-Alt-[F1|F2|F3|F4]
     *  o Switch workspace taking active window: Meta-Ctrl-[F1|F2|F3|F4]
     *  @{ */
    bool handle_pointer_event(MirPointerEvent const* event) override;
    bool handle_touch_event(MirTouchEvent const* event) override;
    bool handle_keyboard_event(MirKeyboardEvent const* event) override;
    /** @} */

    /** @name track events that affect titlebar
     *  @{ */
    void advise_new_window(miral::WindowInfo const& window_info) override;
    void handle_window_ready(miral::WindowInfo& window_info) override;
    void advise_focus_lost(miral::WindowInfo const& info) override;
    void advise_focus_gained(miral::WindowInfo const& info) override;
    void advise_state_change(miral::WindowInfo const& window_info, MirWindowState state) override;
    void advise_resize(miral::WindowInfo const& window_info, Size const& new_size) override;
    void advise_delete_window(miral::WindowInfo const& window_info) override;

    void handle_modify_window(miral::WindowInfo& window_info, miral::WindowSpecification const& modifications) override;
    /** @} */

    /** @name support for CSD invoked sizing and movement
     *  @{ */
    void handle_request_drag_and_drop(miral::WindowInfo& window_info) override;

    void handle_request_move(miral::WindowInfo& window_info, MirInputEvent const* input_event) override;

    void handle_request_resize(
        miral::WindowInfo& window_info, MirInputEvent const* input_event, MirResizeEdge edge) override;
    /** @} */

protected:
    static const int modifier_mask =
        mir_input_event_modifier_alt |
        mir_input_event_modifier_shift |
        mir_input_event_modifier_sym |
        mir_input_event_modifier_ctrl |
        mir_input_event_modifier_meta;

private:
    void toggle(MirWindowState state);

    Point old_cursor{};

    int old_touch_pinch_top = 0;
    int old_touch_pinch_left = 0;
    int old_touch_pinch_width = 0;
    int old_touch_pinch_height = 0;
    bool pinching = false;

    bool pointer_moving = false;
    bool pointer_resizing = false;
    MirPointerButton active_pointer_button;
    unsigned active_pointer_modifiers = 0;
    MirResizeEdge resize_edge = mir_resize_edge_none;
    miral::Window resize_window;
    Point resize_top_left;
    Size resize_size;

    std::shared_ptr<SplashSession> const spinner;

    std::unique_ptr<DecorationProvider> const decoration_provider;

    void end_resize();

    void keep_window_within_constraints(
        miral::WindowInfo const& window_info,
        Displacement& movement,
        Width& new_width,
        Height& new_height) const;

    // Workaround for lp:1627697
    std::chrono::steady_clock::time_point last_resize;

    void advise_adding_to_workspace(
        std::shared_ptr<miral::Workspace> const& workspace,
        std::vector<miral::Window> const& windows) override;

    auto confirm_placement_on_display(
        miral::WindowInfo const& window_info,
        MirWindowState new_state,
        Rectangle const& new_placement) -> Rectangle override;

    // Switch workspace, taking window (if not null)
    void switch_workspace_to(
        std::shared_ptr<miral::Workspace> const& workspace,
        miral::Window const& window = miral::Window{});

    std::shared_ptr<miral::Workspace> active_workspace;
    std::map<int, std::shared_ptr<miral::Workspace>> key_to_workspace;
    std::map<std::shared_ptr<miral::Workspace>, miral::Window> workspace_to_active;

    void apply_workspace_visible_to(miral::Window const& window);

    void apply_workspace_hidden_to(miral::Window const& window);

    void keep_spinner_on_top();
};

#endif //MIRAL_SHELL_FLOATING_WINDOW_MANAGER_H
