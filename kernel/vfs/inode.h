// kern/vfs/inode.h
struct inode {
    union {
        struct device __device_info;
        struct sfs_inode __sfs_inode_info;
    } in_info;
    enum {
        inode_type_device_info = 0x1234,
        inode_type_sfs_inode_info,
    } in_type;
    atomic_t ref_count;
    atomic_t open_count;
    struct fs *in_fs;
    const struct inode_ops *in_ops;
};
