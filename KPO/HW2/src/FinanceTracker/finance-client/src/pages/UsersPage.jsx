import React from "react";
import { useQuery, useMutation } from "@tanstack/react-query";
import { api } from "../api/api";
import {
    Box, Button, Grid, Paper, Stack, TextField, Typography,
} from "@mui/material";
import { useCurrentUser } from "../state/UserContext";

export default function UsersPage() {
    const { user, setUser } = useCurrentUser();
    const q = useQuery({ queryKey: ["users"], queryFn: async () => (await api.get("/users")).data });

    const create = useMutation({
        mutationFn: (payload) => api.post("/users", payload),
        onSuccess: () => q.refetch(),
    });

    return (
        <Stack spacing={3}>
            <Typography variant="h5">👤 Пользователи</Typography>

            <Paper sx={{ p: 2 }}>
                <NewUserForm onSubmit={(p) => create.mutate(p)} loading={create.isPending} />
            </Paper>

            <Grid container spacing={2}>
                {q.data?.map((u) => (
                    <Grid item xs={12} md={6} lg={4} key={u.id}>
                        <Paper
                            sx={{ p: 2, border: user?.id === u.id ? "2px solid #1976d2" : "1px solid #eee" }}
                        >
                            <Typography variant="h6">{u.name}</Typography>
                            <Typography variant="body2" color="text.secondary">{u.email}</Typography>
                            <Button sx={{ mt: 1.5 }} variant="contained" onClick={() => setUser(u)}>
                                Выбрать
                            </Button>
                        </Paper>
                    </Grid>
                ))}
            </Grid>
        </Stack>
    );
}

function NewUserForm({ onSubmit, loading }) {
    const [name, setName] = React.useState("");
    const [email, setEmail] = React.useState("");

    return (
        <Box component="form"
             onSubmit={(e) => { e.preventDefault(); onSubmit({ name, email }); setName(""); setEmail(""); }}
             sx={{ display: "flex", gap: 2 }}
        >
            <TextField label="Имя" value={name} onChange={(e) => setName(e.target.value)} fullWidth />
            <TextField label="Email" value={email} onChange={(e) => setEmail(e.target.value)} fullWidth />
            <Button type="submit" variant="contained" disabled={loading}>Добавить</Button>
        </Box>
    );
}