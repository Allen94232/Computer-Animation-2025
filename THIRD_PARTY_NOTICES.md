# Third-Party Notices and Asset Audit

This file records third-party material that is visibly present in the repository. It is an inventory, not a replacement for the original licenses or a grant of redistribution rights.

## Verified Repository Contents

| Component | Repository location | Follow-up |
| --- | --- | --- |
| RootMotion Final IK / Baker | `Final Project/Assets/Plugins/RootMotion/` | Confirm the Asset Store seat and redistribution terms before making a public release archive. |
| Ready Player Me Core | `Final Project/Assets/Ready Player Me/Core/` | Preserve the upstream notices and check the SDK version's terms before redistribution. |
| Ready Player Me avatar export | `Final Project/Assets/Ready Player Me/Avatars/` | Confirm the avatar and texture usage rights for the intended release. |

The C++ assignments also reference common libraries such as Eigen, GLFW, ImGui, and GLM. Preserve any notices bundled with those dependencies and verify their versions when preparing a binary release.

## Maintainer Checklist

- Keep original license and notice files with vendored dependencies.
- Record the source URL, version, author, and license for newly added assets.
- Do not assume that purchasing or downloading an Asset Store package permits republishing its source files.
- Add project-authored asset ownership information only when it can be verified.
